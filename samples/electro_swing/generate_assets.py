#!/usr/bin/env python3
# /// script
# requires-python = ">=3.14"
# dependencies = [
#     "jinja2>=3.1.6",
# ]
# ///
"""Generate assets.c from sprite TGA frames and a Jinja2 template.

This script:
1. Iterates each subfolder in assets/ as one sprite.
2. Reads all TGA frames in each sprite folder.
3. Builds a shared 256-entry 0x00RRGGBB palette (color_palette).
4. Converts each frame to palette indices.
5. Renders assets.c from assets.c.jinja.
"""

from __future__ import annotations

import argparse
import re
import sys
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import List


@dataclass
class FrameData:
    paletteIndices: List[int]


@dataclass
class SpriteData:
    name: str
    nameTitleCase: str
    nameUpperSnake: str
    width: int
    height: int
    frames: List[FrameData]


def to_lower_camel(name: str) -> str:
    parts = [p for p in re.split(r"[^0-9A-Za-z]+", name) if p]
    if not parts:
        return "sprite"
    first = parts[0].lower()
    rest = "".join(p[:1].upper() + p[1:] for p in parts[1:])
    return first + rest


def to_upper_snake(name: str) -> str:
    text = re.sub(r"[^0-9A-Za-z]+", "_", name)
    text = re.sub(r"_+", "_", text).strip("_")
    if not text:
        return "SPRITE"
    return text.upper()


def read_tga_rgb24(path: Path) -> tuple[int, int, List[int]]:
    data = path.read_bytes()
    if len(data) < 18:
        raise ValueError(f"{path}: too small to be a TGA")

    id_length = data[0]
    color_map_type = data[1]
    image_type = data[2]
    width = int.from_bytes(data[12:14], "little")
    height = int.from_bytes(data[14:16], "little")
    pixel_depth = data[16]
    image_desc = data[17]

    if color_map_type != 0:
        raise ValueError(f"{path}: color-mapped TGAs are not supported")
    if image_type != 2:
        raise ValueError(
            f"{path}: only uncompressed true-color TGAs (type 2) are supported"
        )
    if pixel_depth not in (24, 32):
        raise ValueError(f"{path}: only 24-bit or 32-bit TGAs are supported")

    top_origin = bool(image_desc & 0x20)
    right_origin = bool(image_desc & 0x10)
    bpp = pixel_depth // 8

    pixel_data_start = 18 + id_length
    expected = width * height * bpp
    if len(data) < pixel_data_start + expected:
        raise ValueError(f"{path}: truncated pixel data")

    # Return pixels in display order (top-to-bottom, left-to-right), packed as 0x00RRGGBB.
    pixels: List[int] = []
    for y in range(height):
        sy = y if top_origin else (height - 1 - y)
        for x in range(width):
            sx = (width - 1 - x) if right_origin else x
            i = sy * width + sx
            off = pixel_data_start + i * bpp
            b = data[off]
            g = data[off + 1]
            r = data[off + 2]
            rgb = (r << 16) | (g << 8) | b
            pixels.append(rgb)

    return width, height, pixels


def build_context(assets_dir: Path) -> dict:
    sprite_dirs = sorted([p for p in assets_dir.iterdir() if p.is_dir()])
    if not sprite_dirs:
        raise ValueError(f"No sprite subfolders found in {assets_dir}")

    # Initialize the palette with any manual colors we want that aren't necessarily in sprites
    color_to_index = {
        0x00000000: 0,
        0x00121C18: 1,  # W0 - deep murky base
        0x00243A30: 2,  # W1 - mid water
        0x00486E5C: 3,  # W2 - light streak
        0x0078A082: 4,  # W3 - foam highlight
        0x001C1C1C: 5,  # S0 - dark stone
        0x003C3C3C: 6,  # S1 - mid stone
        0x006E6E6E: 7,  # S2 - light stone
        0x00282C30: 8,  # M0 - dark metal
        0x0078828C: 9,  # M1 - light metal
        0x00965028: 10,  # rust accent
        0x003C78B4: 11,  # A0 - muted blue
        0x005AB4FF: 12,  # A1 - bright cyan
        0x00C8DCFF: 13,  # highlight sparkle
        0x00502814: 14,  # grime / stain
        0x008CB43C: 15,  # moss tint
        0x00FFFFFF: 16,  # Pure white
    }
    palette: List[int] = list(color_to_index.keys())

    sprites: List[SpriteData] = []

    for sprite_dir in sprite_dirs:
        frame_files = sorted(
            [
                p
                for p in sprite_dir.iterdir()
                if p.is_file() and p.suffix.lower() == ".tga"
            ]
        )
        if not frame_files:
            continue

        width = -1
        height = -1
        frames_raw: List[List[int]] = []

        for frame_path in frame_files:
            w, h, pixels = read_tga_rgb24(frame_path)
            if width < 0:
                width, height = w, h
            elif (w, h) != (width, height):
                raise ValueError(
                    f"Sprite '{sprite_dir.name}' has mixed frame dimensions: "
                    f"expected {width}x{height}, got {w}x{h} in {frame_path.name}"
                )

            frames_raw.append(pixels)

            for rgb in pixels:
                if rgb not in color_to_index:
                    if len(palette) >= 256:
                        raise ValueError(
                            f"Palette overflow: more than 256 unique colors after processing {frame_path}"
                        )
                    color_to_index[rgb] = len(palette)
                    palette.append(rgb)

        frames = [
            FrameData(paletteIndices=[color_to_index[c] for c in pixels])
            for pixels in frames_raw
        ]

        sprites.append(
            SpriteData(
                name=sprite_dir.name,
                nameTitleCase=to_lower_camel(sprite_dir.name),
                nameUpperSnake=to_upper_snake(sprite_dir.name),
                width=width,
                height=height,
                frames=frames,
            )
        )

    if not sprites:
        raise ValueError(f"No TGA frames found under {assets_dir}")

    color_palette = [f"0x{rgb:08X}" for rgb in palette]
    while len(color_palette) < 256:
        color_palette.append("0x00000000")

    return {
        "color_palette": color_palette,
        "palette_used": len(palette),
        "sprites": [asdict(s) for s in sprites],
    }


def render_assets(template_path: Path, output_path: Path, context: dict) -> None:
    try:
        from jinja2 import Environment
    except ImportError as exc:
        raise RuntimeError(
            "Missing dependency: jinja2. Install with: pip install jinja2"
        ) from exc

    template_text = template_path.read_text(encoding="utf-8")

    # Keep global trimming off; template uses selective {%- ... -%} controls.
    env = Environment(trim_blocks=False, lstrip_blocks=False)
    rendered = env.from_string(template_text).render(**context)

    output_path.write_text(rendered.rstrip() + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate assets.c from TGA sprite folders"
    )
    parser.add_argument(
        "--assets-dir",
        default="assets",
        help="Assets root directory containing sprite subfolders",
    )
    parser.add_argument("--template", default="assets.c.jinja", help="Template path")
    parser.add_argument("--output", default="assets.c", help="Output C file path")
    args = parser.parse_args()

    base = Path.cwd()
    assets_dir = (base / args.assets_dir).resolve()
    template_path = (base / args.template).resolve()
    output_path = (base / args.output).resolve()

    if not assets_dir.exists():
        raise FileNotFoundError(f"Assets directory not found: {assets_dir}")
    if not template_path.exists():
        raise FileNotFoundError(f"Template file not found: {template_path}")

    context = build_context(assets_dir)
    render_assets(template_path, output_path, context)

    sprite_count = len(context["sprites"])
    palette_count = int(context.get("palette_used", 0))
    print(
        f"Generated {output_path.name}: sprites={sprite_count}, palette_entries_used={palette_count}"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
