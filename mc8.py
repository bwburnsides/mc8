from collections import namedtuple
import subprocess
import pathlib


SOURCE_FILES = (
    "emu/main.cpp",
    "emu/mc8.cpp",
    "emu/dis.cpp",
    "emu/bus.cpp",
)

OUTPUT_NAME = "bin/emulator"


_ParsedArguments = namedtuple("_ParsedArguments", ("is_build", "rom_file"), defaults=(None,))

def main(*, is_build: bool, rom_file: str | None):
    args = ("./bin/emulator", rom_file)

    if is_build:
        print("Building...")
        pathlib.Path("bin/").mkdir(exist_ok=True)
        args = ("g++", *SOURCE_FILES, "-o", OUTPUT_NAME)

    subprocess.run(args)

def _parse_args(argv: list[str]) -> _ParsedArguments:
    import argparse

    argparser = argparse.ArgumentParser()
    subparsers = argparser.add_subparsers(dest="subparser")

    subparsers.add_parser("build")
    subparsers.add_parser("run").add_argument("rom")

    args = argparser.parse_args(argv)

    if args.subparser == "run":
        return _ParsedArguments(is_build=False, rom_file=args.rom)

    return _ParsedArguments(is_build=True)


if __name__ == "__main__":
    import sys

    args = _parse_args(sys.argv[1:])
    main(is_build=args.is_build, rom_file=args.rom_file)
