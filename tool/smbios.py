import struct

data = bytearray()


def add_ram(size_mb, manufacturer, mem_type, speed):
    global data

    strings = [
        "DIMM0",
        manufacturer
    ]

    typ = 17
    length = 0x40
    handle = 0x1100 + len(data)

    header = struct.pack(
        "<BBH",
        typ,
        length,
        handle
    )

    body = bytearray(length - 4)


    # Physical Memory Array Handle
    struct.pack_into(
        "<H",
        body,
        0x00,
        0x1000
    )


    # Memory Error Information Handle
    struct.pack_into(
        "<H",
        body,
        0x02,
        0xFFFF
    )


    # Total Width
    struct.pack_into(
        "<H",
        body,
        0x04,
        64
    )


    # Data Width
    struct.pack_into(
        "<H",
        body,
        0x06,
        64
    )


    # Size MB
    struct.pack_into(
        "<H",
        body,
        0x08,
        size_mb
    )


    # Form Factor = DIMM
    body[0x0A] = 0x09


    # Device Locator string index
    body[0x0C] = 1


    # Bank Locator string index
    body[0x0D] = 0


    # Memory Type
    # DDR4 = 0x1A
    # DDR5 = 0x22
    body[0x0E] = mem_type


    # Type Detail
    struct.pack_into(
        "<H",
        body,
        0x10,
        0
    )


    # Speed MHz
    struct.pack_into(
        "<H",
        body,
        0x11,
        speed
    )


    # Manufacturer string index
    body[0x13] = 2


    # Serial Number string index
    body[0x14] = 0


    # Asset Tag string index
    body[0x15] = 0


    # Part Number string index
    body[0x16] = 0


    # Configured Memory Speed
    struct.pack_into(
        "<H",
        body,
        0x1C,
        speed
    )


    data.extend(header)
    data.extend(body)


    # Strings SMBIOS
    for s in strings:
        data.extend(s.encode())
        data.append(0)


    # Fine stringhe record
    data.append(0)



# RAM 1
add_ram(
    8192,
    "Kingston",
    0x22,
    6000
)


# RAM 2
add_ram(
    8192,
    "Corsair",
    0x22,
    6000
)



# End marker SMBIOS
data.extend(
    struct.pack(
        "<BBH",
        127,
        4,
        0
    )
)

data.extend(b"\0\0")


with open(
    "smbios.bin",
    "wb"
) as f:
    f.write(data)


print("Created smbios.bin")