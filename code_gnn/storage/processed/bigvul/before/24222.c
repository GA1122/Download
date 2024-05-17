ar6000_softmac_update(struct ar6_softc *ar, u8 *eeprom_data, size_t size)
{
    const char *source = "random generated";
    const struct firmware *softmac_entry;
    u8 *ptr_mac;
    switch (ar->arTargetType) {
    case TARGET_TYPE_AR6002:
        ptr_mac = (u8 *)((u8 *)eeprom_data + AR6002_MAC_ADDRESS_OFFSET);
        break;
    case TARGET_TYPE_AR6003:
        ptr_mac = (u8 *)((u8 *)eeprom_data + AR6003_MAC_ADDRESS_OFFSET);
        break;
    default:
	AR_DEBUG_PRINTF(ATH_DEBUG_ERR, ("Invalid Target Type\n"));
        return;
    }
	printk(KERN_DEBUG "MAC from EEPROM %pM\n", ptr_mac);

     
    ptr_mac[0] = 0;
    ptr_mac[1] = 0x03;
    ptr_mac[2] = 0x7F;
    ptr_mac[3] = random32() & 0xff; 
    ptr_mac[4] = random32() & 0xff; 
    ptr_mac[5] = random32() & 0xff; 
    if ((A_REQUEST_FIRMWARE(&softmac_entry, "softmac", ((struct device *)ar->osDevInfo.pOSDevice))) == 0)
    {
        char *macbuf = A_MALLOC_NOWAIT(softmac_entry->size+1);
        if (macbuf) {            
            unsigned int softmac[6];
            memcpy(macbuf, softmac_entry->data, softmac_entry->size);
            macbuf[softmac_entry->size] = '\0';
            if (sscanf(macbuf, "%02x:%02x:%02x:%02x:%02x:%02x", 
                        &softmac[0], &softmac[1], &softmac[2],
                        &softmac[3], &softmac[4], &softmac[5])==6) {
                int i;
                for (i=0; i<6; ++i) {
                    ptr_mac[i] = softmac[i] & 0xff;
                }
                source = "softmac file";
            }
            kfree(macbuf);
        }
        A_RELEASE_FIRMWARE(softmac_entry);
    }
	printk(KERN_DEBUG "MAC from %s %pM\n", source, ptr_mac);
   calculate_crc(ar->arTargetType, eeprom_data);
}