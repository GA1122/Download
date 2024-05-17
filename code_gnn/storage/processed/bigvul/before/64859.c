IW_IMPL(int) iw_read_bmp_file(struct iw_context *ctx, struct iw_iodescr *iodescr)
{
	struct iwbmprcontext rctx;
	struct iw_image img;
	int retval = 0;

	iw_zeromem(&rctx,sizeof(struct iwbmprcontext));
	iw_zeromem(&img,sizeof(struct iw_image));

	rctx.ctx = ctx;
	rctx.img = &img;
	rctx.iodescr = iodescr;

	iw_make_srgb_csdescr_2(&rctx.csdescr);

	rctx.has_fileheader = !iw_get_value(ctx,IW_VAL_BMP_NO_FILEHEADER);
	if(rctx.has_fileheader) {
		if(!iwbmp_read_file_header(&rctx)) goto done;
	}
	if(!iwbmp_read_info_header(&rctx)) goto done;

	iwbmp_set_default_bitfields(&rctx);
	if(rctx.bitfields_nbytes>0) {
		if(!iwbmp_read_bitfields(&rctx)) goto done;
	}

	if(rctx.palette_entries>0) {
		if(!iwbmp_read_palette(&rctx)) goto done;
	}
	if(!iwbmp_read_bits(&rctx)) goto done;

	iw_set_input_image(ctx, &img);

	iwbmpr_misc_config(ctx, &rctx);

	retval = 1;
done:
	if(!retval) {
		iw_set_error(ctx,"BMP read failed");
		iw_free(ctx, img.pixels);
	}
	return retval;
}