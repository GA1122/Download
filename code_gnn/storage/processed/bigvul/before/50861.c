make_boot_catalog(struct archive_write *a)
{
	struct iso9660 *iso9660 = a->format_data;
	unsigned char *block;
	unsigned char *p;
	uint16_t sum, *wp;

	block = wb_buffptr(a);
	memset(block, 0, LOGICAL_BLOCK_SIZE);
	p = block;
	 
	 
	p[0] = 1;
	 
	p[1] = iso9660->el_torito.platform_id;
	 
	p[2] = p[3] = 0;
	 
	if (archive_strlen(&(iso9660->el_torito.id)) > 0)
		strncpy((char *)p+4, iso9660->el_torito.id.s, 23);
	p[27] = 0;
	 
	p[28] = p[29] = 0;
	 
	p[30] = 0x55;
	p[31] = 0xAA;

	sum = 0;
	wp = (uint16_t *)block;
	while (wp < (uint16_t *)&block[32])
		sum += archive_le16dec(wp++);
	set_num_721(&block[28], (~sum) + 1);

	 
	p = &block[32];
	 
	p[0] = 0x88;
	 
	p[1] = iso9660->el_torito.media_type;
	 
	if (iso9660->el_torito.media_type == BOOT_MEDIA_NO_EMULATION)
		set_num_721(&p[2], iso9660->el_torito.boot_load_seg);
	else
		set_num_721(&p[2], 0);
	 
	p[4] = iso9660->el_torito.system_type;
	 
	p[5] = 0;
	 
	if (iso9660->el_torito.media_type == BOOT_MEDIA_NO_EMULATION)
		set_num_721(&p[6], iso9660->el_torito.boot_load_size);
	else
		set_num_721(&p[6], 1);
	 
	set_num_731(&p[8],
	    iso9660->el_torito.boot->file->content.location);
	 
	memset(&p[12], 0, 20);

	return (wb_consume(a, LOGICAL_BLOCK_SIZE));
}