static iw_tmpsample get_raw_sample(struct iw_context *ctx,
	   int x, int y, int channel)
{
	unsigned int v;

	if(channel>=ctx->img1_numchannels_physical) {
		return 1.0;
	}

	if(ctx->img1.sampletype==IW_SAMPLETYPE_FLOATINGPOINT) {
		int rx, ry;
		translate_coords(ctx,x,y,&rx,&ry);
		if(ctx->img1.bit_depth!=32) return 0.0;
		return get_raw_sample_flt32(ctx,rx,ry,channel);
	}

	v = get_raw_sample_int(ctx,x,y,channel);
	return ((double)v) / ctx->img1_ci[channel].maxcolorcode_dbl;
}