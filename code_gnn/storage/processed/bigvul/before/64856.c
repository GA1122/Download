static int decode_v5_header_fields(struct iwbmprcontext *rctx, const iw_byte *buf)
{
	unsigned int intent_bmp_style;
	int intent_iw_style;

	intent_bmp_style = iw_get_ui32le(&buf[108]);
	intent_iw_style = IW_INTENT_UNKNOWN;
	switch(intent_bmp_style) {
		case 1: intent_iw_style = IW_INTENT_SATURATION; break;  
		case 2: intent_iw_style = IW_INTENT_RELATIVE; break;  
		case 4: intent_iw_style = IW_INTENT_PERCEPTUAL; break;  
		case 8: intent_iw_style = IW_INTENT_ABSOLUTE; break;  
	}
	rctx->img->rendering_intent = intent_iw_style;


	return 1;
}
