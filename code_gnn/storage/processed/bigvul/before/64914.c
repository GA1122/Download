static iw_tmpsample get_sample_cvt_to_linear(struct iw_context *ctx,
	   int x, int y, int channel, const struct iw_csdescr *csdescr)
{
	unsigned int v1,v2,v3;
	iw_tmpsample r,g,b;
	int ch;

	ch = ctx->intermed_ci[channel].corresponding_input_channel;

	if(ctx->img1_ci[ch].disable_fast_get_sample) {
		if(ctx->intermed_ci[channel].cvt_to_grayscale) {
			r = x_to_linear_sample(get_raw_sample(ctx,x,y,ch+0),csdescr);
			g = x_to_linear_sample(get_raw_sample(ctx,x,y,ch+1),csdescr);
			b = x_to_linear_sample(get_raw_sample(ctx,x,y,ch+2),csdescr);
			return iw_color_to_grayscale(ctx,r,g,b);
		}
		return x_to_linear_sample(get_raw_sample(ctx,x,y,ch),csdescr);
	}

	if(ctx->intermed_ci[channel].cvt_to_grayscale) {
		v1 = get_raw_sample_int(ctx,x,y,ch+0);
		v2 = get_raw_sample_int(ctx,x,y,ch+1);
		v3 = get_raw_sample_int(ctx,x,y,ch+2);
		r = cvt_int_sample_to_linear(ctx,v1,csdescr);
		g = cvt_int_sample_to_linear(ctx,v2,csdescr);
		b = cvt_int_sample_to_linear(ctx,v3,csdescr);
		return iw_color_to_grayscale(ctx,r,g,b);
	}

	v1 = get_raw_sample_int(ctx,x,y,ch);
	return cvt_int_sample_to_linear(ctx,v1,csdescr);
}