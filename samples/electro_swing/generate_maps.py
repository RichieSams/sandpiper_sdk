#!/usr/bin/env python3
# /// script
# requires-python = ">=3.14"
# dependencies = [
#     "jinja2>=3.1.6",
# ]
# ///
"""Generate maps.c from text map files and a Jinja2 template."""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

TILE_SIZE = 32
ANCHOR_OFFSET = TILE_SIZE // 2

BASE_TILE_MAP = {
    "W": "wallTile",
    "_": "waterTileVertical",
    "A": "anchorTile",
    "D": "waterTileVertical",
}

GOAL_TILES = {
    (0, 0): "goalTopLeftTile",
    (1, 0): "goalTopRightTile",
    (0, 1): "goalBottomLeftTile",
    (1, 1): "goalBottomRightTile",
}


def to_c_identifier(name: str) -> str:
    cleaned = re.sub(r"[^0-9A-Za-z_]", "_", name)
    cleaned = re.sub(r"_+", "_", cleaned).strip("_")
    if not cleaned:
        return "map"
    if cleaned[0].isdigit():
        return f"map_{cleaned}"
    return cleaned


def load_map_lines(path: Path) -> list[str]:
    lines = path.read_text(encoding="utf-8").splitlines()
    if not lines:
        raise ValueError(f"{path.name}: map file is empty")

    width = len(lines[0])
    if width == 0:
        raise ValueError(f"{path.name}: first row is empty")

    for i, line in enumerate(lines, start=1):
        if len(line) != width:
            raise ValueError(
                f"{path.name}: row {i} has width {len(line)}, expected {width}"
            )

    return lines


def parse_goal(
    goal_positions: set[tuple[int, int]], file_name: str
) -> tuple[int, int, int, int]:
    if len(goal_positions) != 4:
        raise ValueError(
            f"{file_name}: expected exactly 4 goal tiles forming one 2x2 block, found {len(goal_positions)}"
        )

    min_x = min(x for x, _ in goal_positions)
    min_y = min(y for _, y in goal_positions)

    expected = {
        (min_x, min_y),
        (min_x + 1, min_y),
        (min_x, min_y + 1),
        (min_x + 1, min_y + 1),
    }
    if goal_positions != expected:
        raise ValueError(f"{file_name}: goal tiles must form a contiguous 2x2 block")

    goal_x0 = min_x * TILE_SIZE
    goal_y0 = min_y * TILE_SIZE
    goal_x1 = (min_x + 2) * TILE_SIZE
    goal_y1 = (min_y + 2) * TILE_SIZE
    return goal_x0, goal_y0, goal_x1, goal_y1


def parse_map_file(path: Path) -> dict:
    lines = load_map_lines(path)
    width_tiles = len(lines[0])
    height_tiles = len(lines)

    map_name = to_c_identifier(path.stem)

    anchors: list[dict[str, int]] = []
    goal_positions: set[tuple[int, int]] = set()
    tiles: list[str] = []

    for y, line in enumerate(lines):
        for x, char in enumerate(line):
            if char == "G":
                goal_positions.add((x, y))
                tiles.append("goalTopLeftTile")
                continue

            tile = BASE_TILE_MAP.get(char)
            if tile is None:
                raise ValueError(
                    f"{path.name}: unsupported tile '{char}' at x={x}, y={y}"
                )

            tiles.append(tile)
            if char == "A":
                anchors.append(
                    {
                        "pos_x": x * TILE_SIZE + ANCHOR_OFFSET,
                        "pos_y": y * TILE_SIZE + ANCHOR_OFFSET,
                    }
                )

    goal_x0, goal_y0, goal_x1, goal_y1 = parse_goal(goal_positions, path.name)

    # Replace placeholder goal tiles with their quadrant-specific tile symbols.
    for x, y in goal_positions:
        rel = (x - (goal_x0 // TILE_SIZE), y - (goal_y0 // TILE_SIZE))
        tile_symbol = GOAL_TILES.get(rel)
        if tile_symbol is None:
            raise ValueError(
                f"{path.name}: invalid goal tile placement at x={x}, y={y}"
            )
        index = y * width_tiles + x
        tiles[index] = tile_symbol

    return {
        "name": map_name,
        "tiles": tiles,
        "anchors": anchors,
        "goal_x0": goal_x0,
        "goal_y0": goal_y0,
        "goal_x1": goal_x1,
        "goal_y1": goal_y1,
        "width": width_tiles,
        "height": height_tiles,
    }


def build_context(maps_dir: Path) -> dict:
    map_files = sorted(p for p in maps_dir.glob("*.txt") if p.is_file())
    if not map_files:
        raise ValueError(f"No map files found in {maps_dir}")

    maps = [parse_map_file(path) for path in map_files]
    return {"maps": maps}


def render_maps(template_path: Path, output_path: Path, context: dict) -> None:
    try:
        from jinja2 import Environment
    except ImportError as exc:
        raise RuntimeError(
            "Missing dependency: jinja2. Run with uv as documented in script header."
        ) from exc

    template_text = template_path.read_text(encoding="utf-8")
    env = Environment(trim_blocks=False, lstrip_blocks=False)
    rendered = env.from_string(template_text).render(**context)
    output_path.write_text(rendered.rstrip() + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Generate maps.c from maps/*.txt and maps.c.jinja"
    )
    parser.add_argument(
        "--maps-dir", default="maps", help="Directory containing *.txt map files"
    )
    parser.add_argument(
        "--template", default="maps.c.jinja", help="Jinja template file"
    )
    parser.add_argument("--output", default="maps.c", help="Output C source file")
    args = parser.parse_args()

    base = Path.cwd()
    maps_dir = (base / args.maps_dir).resolve()
    template_path = (base / args.template).resolve()
    output_path = (base / args.output).resolve()

    if not maps_dir.exists():
        raise FileNotFoundError(f"Maps directory not found: {maps_dir}")
    if not template_path.exists():
        raise FileNotFoundError(f"Template file not found: {template_path}")

    context = build_context(maps_dir)
    render_maps(template_path, output_path, context)
    print(f"Generated {output_path.name} from {len(context['maps'])} map file(s)")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except Exception as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        raise SystemExit(1)
