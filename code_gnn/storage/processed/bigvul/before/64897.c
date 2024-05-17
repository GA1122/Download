static iw_tmpsample cvt_int_sample_to_linear(struct iw_context *ctx,
	unsigned int v, const struct iw_csdescr *csdescr)
{
	iw_tmpsample s;

	if(csdescr->cstype==IW_CSTYPE_LINEAR) {
		return ((double)v) / ctx->input_maxcolorcode;
	}
	else if(ctx->input_color_corr_table) {
		return ctx->input_color_corr_table[v];
	}

	s = ((double)v) / ctx->input_maxcolorcode;
	return x_to_linear_sample(s,csdescr);
}
