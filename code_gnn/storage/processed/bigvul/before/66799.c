static int iwgif_read_screen_descriptor(struct iwgifrcontext *rctx)
{
	int has_global_ct;
	int global_ct_size;
	int aspect_ratio_code;

	if(!iwgif_read(rctx,rctx->rbuf,7)) return 0;
	rctx->screen_width = (int)iw_get_ui16le(&rctx->rbuf[0]);
	rctx->screen_height = (int)iw_get_ui16le(&rctx->rbuf[2]);

	has_global_ct = (int)((rctx->rbuf[4]>>7)&0x01);

	if(has_global_ct) {
		global_ct_size = (int)(rctx->rbuf[4]&0x07);
		rctx->colortable.num_entries = 1<<(1+global_ct_size);

		rctx->bg_color_index = (int)rctx->rbuf[5];
		if(rctx->bg_color_index < rctx->colortable.num_entries)
			rctx->has_bg_color = 1;
	}

	aspect_ratio_code = (int)rctx->rbuf[6];
	if(aspect_ratio_code!=0) {
		rctx->img->density_code = IW_DENSITY_UNITS_UNKNOWN;
		rctx->img->density_x = 64000.0/(double)(aspect_ratio_code + 15);
		rctx->img->density_y = 1000.0;
	}

	return 1;
}
