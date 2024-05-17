static void iw_process_bkgd_label(struct iw_context *ctx)
{
	int ret;
	int k;
	struct iw_color clr;
	double maxcolor;
	unsigned int tmpu;

	if(!(ctx->output_profile&IW_PROFILE_PNG_BKGD) &&
		!(ctx->output_profile&IW_PROFILE_RGB8_BKGD) &&
		!(ctx->output_profile&IW_PROFILE_RGB16_BKGD))
	{
		return;
	}

	ret = get_output_bkgd_label_lin(ctx,&clr);
	if(!ret) return;

	if(ctx->to_grayscale) {
		iw_tmpsample g;
		g = iw_color_to_grayscale(ctx, clr.c[0], clr.c[1], clr.c[2]);
		clr.c[0] = clr.c[1] = clr.c[2] = g;
	}

	if(ctx->output_profile&IW_PROFILE_RGB8_BKGD) {
		maxcolor=255.0;
	}
	else if(ctx->output_profile&IW_PROFILE_RGB16_BKGD) {
		maxcolor=65535.0;
	}
	else if(ctx->img2.bit_depth==8) {
		maxcolor=255.0;
	}
	else if(ctx->img2.bit_depth==16) {
		maxcolor=65535.0;
	}
	else {
		return;
	}

	for(k=0;k<3;k++) {
		tmpu = calc_sample_convert_from_linear(ctx, clr.c[k], &ctx->img2cs, maxcolor);
		ctx->img2.bkgdlabel.c[k] = ((double)tmpu)/maxcolor;
	}
	tmpu = iw_scale_to_int(clr.c[3],(unsigned int)maxcolor);
	ctx->img2.bkgdlabel.c[3] = ((double)tmpu)/maxcolor;

	ctx->img2.has_bkgdlabel = 1;
}