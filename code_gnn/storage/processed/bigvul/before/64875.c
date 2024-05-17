static int iwbmp_write_bmp_v45header_fields(struct iwbmpwcontext *wctx)
{
	iw_byte header[124];
	unsigned int intent_bmp_style;

	iw_zeromem(header,sizeof(header));

	if(wctx->uses_bitfields) {
		iw_set_ui32le(&header[40],wctx->bf_mask[0]);
		iw_set_ui32le(&header[44],wctx->bf_mask[1]);
		iw_set_ui32le(&header[48],wctx->bf_mask[2]);
		iw_set_ui32le(&header[52],wctx->bf_mask[3]);
	}

	if(wctx->csdescr.cstype==IW_CSTYPE_SRGB && !wctx->no_cslabel)
		iw_set_ui32le(&header[56],IWBMPCS_SRGB);
	else
		iw_set_ui32le(&header[56],IWBMPCS_DEVICE_RGB);

	switch(wctx->img->rendering_intent) {
	case IW_INTENT_PERCEPTUAL: intent_bmp_style = 4; break;
	case IW_INTENT_RELATIVE:   intent_bmp_style = 2; break;
	case IW_INTENT_SATURATION: intent_bmp_style = 1; break;
	case IW_INTENT_ABSOLUTE:   intent_bmp_style = 8; break;
	default: intent_bmp_style = 4;
	}
	iw_set_ui32le(&header[108],intent_bmp_style);

	iwbmp_write(wctx,&header[40],124-40);
	return 1;
}