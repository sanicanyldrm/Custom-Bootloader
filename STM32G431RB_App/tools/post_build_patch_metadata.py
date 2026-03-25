#!/usr/bin/env python3
import argparse
import os
import re
import struct
import subprocess
import sys
import zlib

# BootImageMetadata_t field offsets
IMAGE_SIZE_OFFSET_IN_METADATA = 16
IMAGE_CRC_OFFSET_IN_METADATA  = 20
METADATA_SIZE_BYTES           = 32  # 8 x uint32_t

EXCLUDED_SECTION_NAMES = {
    ".app_metadata",
}


def write_u32_le(buf: bytearray, offset: int, value: int) -> None:
    buf[offset:offset + 4] = struct.pack("<I", value)


def read_u32_le(buf: bytearray, offset: int) -> int:
    return struct.unpack("<I", buf[offset:offset + 4])[0]


def run_command(cmd):
    result = subprocess.run(cmd, capture_output=True, text=True)
    if result.returncode != 0:
        print("ERROR: command failed")
        print("Command:", " ".join(cmd))
        print("STDOUT:")
        print(result.stdout)
        print("STDERR:")
        print(result.stderr)
        raise RuntimeError("External command failed")
    return result.stdout


def parse_objdump_sections(elf_path: str, objdump_cmd: str):
    """
    Parse 'arm-none-eabi-objdump -h <elf>' output.

    We use LMA to determine which bytes are physically stored in flash.
    This correctly includes sections like .data whose VMA is in RAM but
    whose LMA is in flash.
    """
    text = run_command([objdump_cmd, "-h", elf_path])

    sections = []
    lines = text.splitlines()

    i = 0
    while i < len(lines):
        line = lines[i]

        # Typical header line:
        #  0 .isr_vector   00000188  08008000  08008000  00010000  2**0
        m = re.match(
            r"^\s*(\d+)\s+(\S+)\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s+([0-9A-Fa-f]{8})\s+2\*\*\d+\s*$",
            line
        )
        if m is not None:
            sec_name = m.group(2)
            sec_size = int(m.group(3), 16)
            sec_vma  = int(m.group(4), 16)
            sec_lma  = int(m.group(5), 16)
            sec_file_off = int(m.group(6), 16)

            flags_line = ""
            if (i + 1) < len(lines):
                flags_line = lines[i + 1].strip()

            flags = {flag.strip() for flag in flags_line.split(",") if flag.strip()}

            sections.append({
                "name": sec_name,
                "size": sec_size,
                "vma": sec_vma,
                "lma": sec_lma,
                "file_off": sec_file_off,
                "flags": flags,
            })

            i += 2
            continue

        i += 1

    return sections


def determine_real_image_size(elf_path: str,
                              objdump_cmd: str,
                              app_start: int,
                              metadata_addr: int) -> int:
    """
    Determine the real application image size using ELF sections.

    Included:
      - sections with CONTENTS + ALLOC + LOAD
      - physically stored in flash (LMA based)
      - located in [app_start, metadata_addr)
      - excluding .app_metadata

    Excluded:
      - .app_metadata
      - anything after metadata
      - sections not physically present in the image
    """
    sections = parse_objdump_sections(elf_path, objdump_cmd)

    included_sections = []
    image_end = app_start

    for sec in sections:
        sec_name = sec["name"]
        sec_size = sec["size"]
        sec_lma = sec["lma"]
        sec_flags = sec["flags"]

        if sec_size == 0:
            continue

        if sec_name in EXCLUDED_SECTION_NAMES:
            continue

        if "CONTENTS" not in sec_flags:
            continue

        if "ALLOC" not in sec_flags:
            continue

        if "LOAD" not in sec_flags:
            continue

        sec_end = sec_lma + sec_size

        # Section must start inside app image region before metadata
        if sec_lma < app_start:
            continue

        if sec_lma >= metadata_addr:
            continue

        # Do not allow section to overlap metadata
        if sec_end > metadata_addr:
            print(f"ERROR: section '{sec_name}' overlaps metadata area.")
            print(f"Section LMA : 0x{sec_lma:08X}")
            print(f"Section end : 0x{sec_end:08X}")
            print(f"Metadata    : 0x{metadata_addr:08X}")
            raise RuntimeError("Section overlaps metadata")

        included_sections.append(sec)

        if sec_end > image_end:
            image_end = sec_end

    if image_end <= app_start:
        raise RuntimeError("No loadable flash sections found before metadata.")

    image_size = image_end - app_start

    print("Included sections for CRC/image size:")
    for sec in included_sections:
        print(
            f"  {sec['name']:<20} "
            f"LMA=0x{sec['lma']:08X} "
            f"SIZE=0x{sec['size']:08X} "
            f"END=0x{(sec['lma'] + sec['size']):08X}"
        )

    print(f"Calculated real image end  : 0x{image_end:08X}")
    print(f"Calculated real image size : {image_size} bytes (0x{image_size:08X})")

    return image_size


def create_hex_from_bin(objcopy_cmd: str, bin_path: str, hex_path: str, app_start: int) -> None:
    cmd = [
        objcopy_cmd,
        "-I", "binary",
        "-O", "ihex",
        "--change-addresses", f"0x{app_start:08X}",
        bin_path,
        hex_path,
    ]
    run_command(cmd)


def main() -> int:
    parser = argparse.ArgumentParser(description="Patch STM32 app metadata using real ELF image size")
    parser.add_argument("--elf", required=True, help="Input ELF file path")
    parser.add_argument("--bin", required=True, help="Input BIN file path")
    parser.add_argument("--app-start", required=True, help="Application start address, e.g. 0x08008000")
    parser.add_argument("--metadata-addr", required=True, help="Metadata absolute flash address, e.g. 0x0801FF00")
    parser.add_argument("--out-bin", required=False, help="Output patched BIN path")
    parser.add_argument("--out-hex", required=False, help="Optional output patched HEX path")
    parser.add_argument("--objdump", default="arm-none-eabi-objdump", help="objdump executable")
    parser.add_argument("--objcopy", default="arm-none-eabi-objcopy", help="objcopy executable")
    args = parser.parse_args()

    elf_path = args.elf
    bin_path = args.bin
    out_bin_path = args.out_bin if args.out_bin else os.path.splitext(bin_path)[0] + "_patched.bin"
    out_hex_path = args.out_hex

    app_start = int(args.app_start, 0)
    metadata_addr = int(args.metadata_addr, 0)

    if metadata_addr <= app_start:
        print("ERROR: metadata address must be greater than app start.")
        return 1

    if not os.path.isfile(elf_path):
        print(f"ERROR: ELF file not found: {elf_path}")
        return 1

    if not os.path.isfile(bin_path):
        print(f"ERROR: BIN file not found: {bin_path}")
        return 1

    metadata_offset = metadata_addr - app_start
    size_field_offset = metadata_offset + IMAGE_SIZE_OFFSET_IN_METADATA
    crc_field_offset = metadata_offset + IMAGE_CRC_OFFSET_IN_METADATA

    try:
        real_image_size = determine_real_image_size(
            elf_path=elf_path,
            objdump_cmd=args.objdump,
            app_start=app_start,
            metadata_addr=metadata_addr,
        )
    except Exception as exc:
        print(f"ERROR while determining real image size: {exc}")
        return 1

    with open(bin_path, "rb") as f:
        image = bytearray(f.read())

    if len(image) < (metadata_offset + METADATA_SIZE_BYTES):
        print("ERROR: BIN is smaller than metadata location.")
        print(f"BIN size         : {len(image)} bytes")
        print(f"Metadata offset  : 0x{metadata_offset:08X}")
        print(f"Needed minimum   : {metadata_offset + METADATA_SIZE_BYTES} bytes")
        return 1

    if real_image_size > metadata_offset:
        print("ERROR: real image size reaches metadata area.")
        print(f"Real image size  : 0x{real_image_size:08X}")
        print(f"Metadata offset  : 0x{metadata_offset:08X}")
        return 1

    # Patch metadata
    write_u32_le(image, size_field_offset, real_image_size)
    write_u32_le(image, crc_field_offset, 0)

    # CRC only on real application image, metadata excluded
    crc_region = image[0:real_image_size]
    image_crc = zlib.crc32(crc_region) & 0xFFFFFFFF

    write_u32_le(image, crc_field_offset, image_crc)

    with open(out_bin_path, "wb") as f:
        f.write(image)

    print("Metadata patch completed successfully.")
    print(f"Input ELF file         : {elf_path}")
    print(f"Input BIN file         : {bin_path}")
    print(f"Output BIN file        : {out_bin_path}")
    print(f"App start              : 0x{app_start:08X}")
    print(f"Metadata address       : 0x{metadata_addr:08X}")
    print(f"Metadata offset        : 0x{metadata_offset:08X}")
    print(f"Real image size        : {real_image_size} bytes (0x{real_image_size:08X})")
    print(f"Image CRC32            : 0x{image_crc:08X}")
    print(f"Patched size readback  : 0x{read_u32_le(image, size_field_offset):08X}")
    print(f"Patched crc readback   : 0x{read_u32_le(image, crc_field_offset):08X}")

    if out_hex_path:
        try:
            create_hex_from_bin(
                objcopy_cmd=args.objcopy,
                bin_path=out_bin_path,
                hex_path=out_hex_path,
                app_start=app_start
            )
            print(f"Output HEX file        : {out_hex_path}")
        except Exception as exc:
            print(f"ERROR while creating HEX: {exc}")
            return 1

    return 0


if __name__ == "__main__":
    sys.exit(main())