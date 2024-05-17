rdp_out_ts_order_capabilityset(STREAM s)
{
	uint8 order_caps[32];
	uint16 orderflags = 0;
	uint32 cachesize = 0;

	orderflags |= (NEGOTIATEORDERSUPPORT | ZEROBOUNDSDELTASSUPPORT);	 
	orderflags |= COLORINDEXSUPPORT;

	memset(order_caps, 0, 32);

	order_caps[TS_NEG_DSTBLT_INDEX] = 1;
	order_caps[TS_NEG_PATBLT_INDEX] = 1;
	order_caps[TS_NEG_SCRBLT_INDEX] = 1;
	order_caps[TS_NEG_LINETO_INDEX] = 1;
	order_caps[TS_NEG_MULTI_DRAWNINEGRID_INDEX] = 1;
	order_caps[TS_NEG_POLYLINE_INDEX] = 1;
	order_caps[TS_NEG_INDEX_INDEX] = 1;

	if (g_bitmap_cache)
		order_caps[TS_NEG_MEMBLT_INDEX] = 1;

	if (g_desktop_save)
	{
		cachesize = 230400;
		order_caps[TS_NEG_SAVEBITMAP_INDEX] = 1;
	}

	if (g_polygon_ellipse_orders)
	{
		order_caps[TS_NEG_POLYGON_SC_INDEX] = 1;
		order_caps[TS_NEG_POLYGON_CB_INDEX] = 1;
		order_caps[TS_NEG_ELLIPSE_SC_INDEX] = 1;
		order_caps[TS_NEG_ELLIPSE_CB_INDEX] = 1;
	}

	out_uint16_le(s, RDP_CAPSET_ORDER);
	out_uint16_le(s, RDP_CAPLEN_ORDER);
	out_uint8s(s, 16);	 
	out_uint8s(s, 4);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 20);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 1);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, orderflags);	 
	out_uint8p(s, order_caps, 32);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 0);	 
	out_uint32_le(s, 0);	 
	out_uint32_le(s, cachesize);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 0);	 
	out_uint16_le(s, 1252);	 
	out_uint16_le(s, 0);	 
}