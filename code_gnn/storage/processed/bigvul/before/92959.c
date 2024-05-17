process_bmpcache(STREAM s)
{
	RD_HBITMAP bitmap;
	uint16 cache_idx, size;
	uint8 cache_id, width, height, bpp, Bpp;
	uint8 *data, *bmpdata;
	uint16 bufsize, pad2, row_size, final_size;
	uint8 pad1;

	pad2 = row_size = final_size = 0xffff;	 

	in_uint8(s, cache_id);
	in_uint8(s, pad1);	 
	in_uint8(s, width);
	in_uint8(s, height);
	in_uint8(s, bpp);
	Bpp = (bpp + 7) / 8;
	in_uint16_le(s, bufsize);	 
	in_uint16_le(s, cache_idx);

	if (g_rdp_version >= RDP_V5)
	{
		size = bufsize;
	}
	else
	{

		 
		in_uint16_le(s, pad2);	 
		in_uint16_le(s, size);
		  
		in_uint16_le(s, row_size);
		in_uint16_le(s, final_size);

	}
	in_uint8p(s, data, size);
	logger(Graphics, Debug,
	       "process_bmpcache(), cx=%d, cy=%d, id=%d, idx=%d, bpp=%d, size=%d, pad1=%d, bufsize=%d, pad2=%d, rs=%d, fs=%d",
	       width, height, cache_id, cache_idx, bpp, size, pad1, bufsize, pad2, row_size,
	       final_size);

	bmpdata = (uint8 *) xmalloc(width * height * Bpp);

	if (bitmap_decompress(bmpdata, width, height, data, size, Bpp))
	{
		bitmap = ui_create_bitmap(width, height, bmpdata);
		cache_put_bitmap(cache_id, cache_idx, bitmap);
	}
	else
	{
		logger(Graphics, Error, "process_bmpcache(), Failed to decompress bitmap data");
	}

	xfree(bmpdata);
}