static void ide_atapi_identify(IDEState *s)
{
    uint16_t *p;

    p = (uint16_t *)s->identify_data;
    if (s->identify_set) {
        goto fill_buffer;
    }
    memset(p, 0, sizeof(s->identify_data));

     
    put_le16(p + 0, (2 << 14) | (5 << 8) | (1 << 7) | (2 << 5) | (0 << 0));
    padstr((char *)(p + 10), s->drive_serial_str, 20);  
    put_le16(p + 20, 3);  
    put_le16(p + 21, 512);  
    put_le16(p + 22, 4);  
    padstr((char *)(p + 23), s->version, 8);  
    padstr((char *)(p + 27), s->drive_model_str, 40);  
    put_le16(p + 48, 1);  
#ifdef USE_DMA_CDROM
    put_le16(p + 49, 1 << 9 | 1 << 8);  
    put_le16(p + 53, 7);  
    put_le16(p + 62, 7);   
    put_le16(p + 63, 7);   
#else
    put_le16(p + 49, 1 << 9);  
    put_le16(p + 53, 3);  
    put_le16(p + 63, 0x103);  
#endif
    put_le16(p + 64, 3);  
    put_le16(p + 65, 0xb4);  
    put_le16(p + 66, 0xb4);  
    put_le16(p + 67, 0x12c);  
    put_le16(p + 68, 0xb4);  

    put_le16(p + 71, 30);  
    put_le16(p + 72, 30);  

    if (s->ncq_queues) {
        put_le16(p + 75, s->ncq_queues - 1);
         
        put_le16(p + 76, (1 << 8));
    }

    put_le16(p + 80, 0x1e);  
    if (s->wwn) {
        put_le16(p + 84, (1 << 8));  
        put_le16(p + 87, (1 << 8));  
    }

#ifdef USE_DMA_CDROM
    put_le16(p + 88, 0x3f | (1 << 13));  
#endif

    if (s->wwn) {
         
        put_le16(p + 108, s->wwn >> 48);
        put_le16(p + 109, s->wwn >> 32);
        put_le16(p + 110, s->wwn >> 16);
        put_le16(p + 111, s->wwn);
    }

    s->identify_set = 1;

fill_buffer:
    memcpy(s->io_buffer, p, sizeof(s->identify_data));
}
