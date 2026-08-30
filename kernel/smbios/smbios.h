#ifndef SMBIOS_H
#define SMBIOS_H

#include "screen/screen.h"
#include "types.h"
#include "libs/string/string.h"
#include "limine.h"
#include "screen/log.h"


struct smbios_header
{
    uint8_t type;
    uint8_t length;
    uint16_t handle;
};


struct smbios_type4
{
    struct smbios_header header;

    uint8_t socket;
    uint8_t processor_type;
    uint8_t processor_family;
    uint8_t manufacturer;

    uint64_t processor_id;

    uint8_t version;
}__attribute__((packed));



struct smbios_entry_point_64
{
    char anchor[5];
    uint8_t checksum;
    uint8_t length;
    uint8_t major_version;
    uint8_t minor_version;
    uint8_t docrev;
    uint8_t revision;
    uint8_t reserved;

    uint32_t max_structure_size;
    uint64_t table_address;

    uint32_t number_of_structures;
}__attribute__((packed));



struct smbios_entry_point_32
{
    char anchor[4];

    uint8_t checksum;
    uint8_t length;

    uint8_t major_version;
    uint8_t minor_version;

    uint16_t max_structure_size;

    uint8_t entry_point_revision;
    uint8_t formatted_area[5];

    char intermediate_anchor[5];

    uint8_t intermediate_checksum;

    uint16_t table_length;

    uint32_t table_address;

    uint16_t number_of_structures;

    uint8_t bcd_revision;

}__attribute__((packed));



struct smbios_type17
{
    struct smbios_header header;


    uint16_t physical_memory_array_handle;

    uint16_t memory_error_information_handle;


    uint16_t total_width;

    uint16_t data_width;


    uint16_t size;          // offset 0x0C


    uint8_t form_factor;    // 0x0E

    uint8_t device_set;     // 0x0F


    uint8_t device_locator; // 0x10

    uint8_t bank_locator;   // 0x11


    uint8_t memory_type;    // 0x12

    uint16_t type_detail;   // 0x13


    uint16_t speed;         // 0x15


    uint8_t manufacturer;   // 0x17

    uint8_t serial_number;  // 0x18

    uint8_t asset_tag;      // 0x19

    uint8_t part_number;    // 0x1A


    uint8_t attributes;     // 0x1B


    uint32_t extended_size; // 0x1C


    uint16_t configured_speed; // 0x20

}__attribute__((packed));


static struct smbios_header *smbios_next(struct smbios_header *hdr);

int smbios_init(struct limine_smbios_response *smb, booln print_info);

void *smbios_typeread(int type);

char *smbios_get_string(struct smbios_header *hdr, uint8_t index);

void *smbios_typeread_next(int type, void *previous);


#endif