rdpdr_send_client_device_list_announce(void)
{
	 
	uint32 bloblen, disklen, flags;
	size_t i;
	STREAM s;
	PRINTER *printerinfo;
	DISK_DEVICE *diskinfo;
	struct stream drv = { 0 }, prt =
	{
	0};

	s = channel_init(rdpdr_channel, announcedata_size());
	out_uint16_le(s, RDPDR_CTYP_CORE);
	out_uint16_le(s, PAKID_CORE_DEVICE_LIST_ANNOUNCE);

	out_uint32_le(s, g_num_devices);

	for (i = 0; i < g_num_devices; i++)	 
	{
		out_uint32_le(s, g_rdpdr_device[i].device_type);
		out_uint32_le(s, i);	 
		out_uint8p(s, g_rdpdr_device[i].name, 8);	 
		switch (g_rdpdr_device[i].device_type)
		{
			case DEVICE_TYPE_DISK:
				diskinfo = (DISK_DEVICE *) g_rdpdr_device[i].pdevice_data;

				 

				disklen = strlen(diskinfo->name) + 1;

				out_uint32_le(s, disklen);	 
				out_uint8p(s, diskinfo->name, disklen);	 
				break;

			case DEVICE_TYPE_PRINTER:
				printerinfo = (PRINTER *) g_rdpdr_device[i].pdevice_data;

				s_realloc(&prt, 512 * 4);
				s_reset(&prt);
				out_utf16s(&prt, printerinfo->printer);
				s_mark_end(&prt);

				s_realloc(&drv, 512 * 4);
				s_reset(&drv);
				out_utf16s(&drv, printerinfo->driver);
				s_mark_end(&drv);

				bloblen = printerinfo->bloblen;
				flags = 0;
				if (printerinfo->default_printer)
					flags |= RDPDR_PRINTER_ANNOUNCE_FLAG_DEFAULTPRINTER;

				out_uint32_le(s, 24 + s_length(&drv) + s_length(&prt) + bloblen);	 
				out_uint32_le(s, flags);	 
				out_uint32_le(s, 0);	 
				out_uint32_le(s, 0);	 
				out_uint32_le(s, s_length(&drv));	 
				out_uint32_le(s, s_length(&prt));	 
				out_uint32_le(s, bloblen);	 
				out_stream(s, &drv);	 
				out_stream(s, &prt);	 
				out_uint8a(s, printerinfo->blob, bloblen);	 

				if (printerinfo->blob)
					xfree(printerinfo->blob);	 
				break;
			default:
				out_uint32(s, 0);
		}
	}

	s_mark_end(s);
	channel_send(s, rdpdr_channel);
}
