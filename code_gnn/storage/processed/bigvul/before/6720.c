static void ide_cfata_metadata_inquiry(IDEState *s)
{
    uint16_t *p;
    uint32_t spd;

    p = (uint16_t *) s->io_buffer;
    memset(p, 0, 0x200);
    spd = ((s->mdata_size - 1) >> 9) + 1;

    put_le16(p + 0, 0x0001);			 
    put_le16(p + 1, 0x0000);			 
    put_le16(p + 2, s->media_changed);		 
    put_le16(p + 3, s->mdata_size & 0xffff);	 
    put_le16(p + 4, s->mdata_size >> 16);	 
    put_le16(p + 5, spd & 0xffff);		 
    put_le16(p + 6, spd >> 16);			 
}
