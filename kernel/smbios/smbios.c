#include "smbios.h"
#include "libs/string/string.h"

static void *smbios_table = 0;


int smbios_init(struct limine_smbios_response *smb, booln print_info)
{
    if(smb->entry_64)
    {
        info("SMBIOS Version: 3.x");

        struct smbios_entry_point_64 *entry =
            (struct smbios_entry_point_64 *)smb->entry_64;

        smbios_table = (void *)entry->table_address;
    }
    else if(smb->entry_32)
    {
        info("SMBIOS Version: 2.x");

        struct smbios_entry_point_32 *entry =
            (struct smbios_entry_point_32 *)smb->entry_32;

        smbios_table = (void *)(uint64_t)entry->table_address;
    }
    else
    {
        return -1;
    }


    if(!smbios_table)
        return -2;


    if(print_info)
    {

        struct smbios_type4 *cpu =
            (struct smbios_type4 *)smbios_typeread(4);


        if(cpu)
        {
            char *name =
                smbios_get_string(
                    (struct smbios_header *)cpu,
                    cpu->version
                );

            if(name)
            {
                print("\nCPU: ",0xFFFFFF);
                print(name,0xFFFFFF);
            }
        }



        struct smbios_type17 *ram = 0;

        int count = 0;


        while(count < 16)
        {

            ram = smbios_typeread_next(17, ram);


            if(!ram)
                break;


            char size[32];
            char speed[32];

            if(ram->size == 0)
            {
                count++;
                continue;
            }

            uint64_to_string(
                ram->size,
                size
            );

            uint64_to_string(
                ram->speed,
                speed
            );


            print("\nRAM: ",0xFFFFFF);
            print(size,0xFFFFFF);
            print(" MB",0xFFFFFF);


            char *manu =
                smbios_get_string(
                    (struct smbios_header *)ram,
                    ram->manufacturer
                );


            if(manu)
            {
                print(" ",0xFFFFFF);
                print(manu,0xFFFFFF);
            }


            print(" ",0xFFFFFF);
            print(speed,0xFFFFFF);
            print(" MHz",0xFFFFFF);



            print(" ",0xFFFFFF);


            if(ram->memory_type == 0x1A)
                print("DDR4",0xFFFFFF);

            else if(ram->memory_type == 0x22)
                print("DDR5",0xFFFFFF);

            else
                print("Unknown",0xFFFFFF);



            count++;
        }
    }


    return 0;
}



void *smbios_typeread(int type)
{
    return smbios_typeread_next(type,0);
}



void *smbios_typeread_next(int type, void *previous)
{
    if(smbios_table == 0)
        return 0;


    struct smbios_header *hdr;


    if(previous == 0)
        hdr = (struct smbios_header *)smbios_table;
    else
        hdr = smbios_next((struct smbios_header *)previous);


    while(1)
    {
        if(hdr->type == 127)
            return 0;


        if(hdr->length < 4)
            return 0;


        if(hdr->type == type)
            return hdr;


        hdr = smbios_next(hdr);
    }
}



static struct smbios_header *smbios_next(struct smbios_header *hdr)
{

    uint8_t *ptr =
        (uint8_t *)hdr + hdr->length;


    for(int i=0;i<512;i++)
    {
        if(ptr[0]==0 && ptr[1]==0)
        {
            ptr += 2;
            return (struct smbios_header *)ptr;
        }

        ptr++;
    }


    return 0;
}



char *smbios_get_string(
    struct smbios_header *hdr,
    uint8_t index
)
{

    if(index==0)
        return 0;


    uint8_t *ptr =
        (uint8_t *)hdr + hdr->length;


    int current=1;


    while(current < index)
    {

        while(*ptr)
            ptr++;

        ptr++;

        current++;
    }


    if(*ptr==0)
        return 0;


    return (char *)ptr;
}