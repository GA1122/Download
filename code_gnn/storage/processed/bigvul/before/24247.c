void calculate_crc(u32 TargetType, u8 *eeprom_data)
{
    u16 *ptr_crc;
    u16 *ptr16_eeprom;
    u16 checksum;
    u32 i;
    u32 eeprom_size;

    if (TargetType == TARGET_TYPE_AR6001)
    {
        eeprom_size = 512;
        ptr_crc = (u16 *)eeprom_data;
    }
    else if (TargetType == TARGET_TYPE_AR6003)
    {
        eeprom_size = 1024;
        ptr_crc = (u16 *)((u8 *)eeprom_data + 0x04);
    }
    else
    {
        eeprom_size = 768;
        ptr_crc = (u16 *)((u8 *)eeprom_data + 0x04);
    }


    *ptr_crc = 0;

    checksum = 0;
    ptr16_eeprom = (u16 *)eeprom_data;
    for (i = 0;i < eeprom_size; i += 2)
    {
        checksum = checksum ^ (*ptr16_eeprom);
        ptr16_eeprom++;
    }
    checksum = 0xFFFF ^ checksum;
    *ptr_crc = checksum;
}