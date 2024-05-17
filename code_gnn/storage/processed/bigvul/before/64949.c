static void put_sample_convert_from_linear(struct iw_context *ctx, iw_tmpsample samp_lin,
	   int x, int y, int channel, const struct iw_csdescr *csdescr)
{
	double s_lin_floor_1, s_lin_ceil_1;
	double s_cvt_floor_full, s_cvt_ceil_full;
	double d_floor, d_ceil;
	int is_exact;
	double s_full;
	int ditherfamily;
	int dd;  

	if(samp_lin<0.0) samp_lin=0.0;
	if(samp_lin>1.0) samp_lin=1.0;

	if(ctx->img2_ci[channel].use_nearest_color_table) {
		s_full = get_final_sample_using_nc_tbl(ctx,samp_lin);
		goto okay;
	}

	ditherfamily=ctx->img2_ci[channel].ditherfamily;

	if(ditherfamily==IW_DITHERFAMILY_ERRDIFF) {
		samp_lin += ctx->dither_errors[0][x];
		if(samp_lin>1.0) samp_lin=1.0;
		else if(samp_lin<0.0) samp_lin=0.0;
	}

	is_exact = get_nearest_valid_colors(ctx,samp_lin,csdescr,
		&s_lin_floor_1, &s_lin_ceil_1,
		&s_cvt_floor_full, &s_cvt_ceil_full,
		ctx->img2_ci[channel].maxcolorcode_dbl, ctx->img2_ci[channel].color_count);

	if(is_exact) {
		s_full = s_cvt_floor_full;

		if(ditherfamily==IW_DITHERFAMILY_RANDOM) {
			(void)iwpvt_prng_rand(ctx->prng);
		}
		goto okay;
	}

	d_floor = samp_lin-s_lin_floor_1;
	d_ceil  = s_lin_ceil_1-samp_lin;

	if(ditherfamily==IW_DITHERFAMILY_NONE) {
		if(d_ceil<=d_floor) s_full=s_cvt_ceil_full;
		else s_full=s_cvt_floor_full;
	}
	else if(ditherfamily==IW_DITHERFAMILY_ERRDIFF) {
		if(d_ceil<=d_floor) {
			iw_errdiff_dither(ctx,ctx->img2_ci[channel].dithersubtype,-d_ceil,x,y);
			s_full=s_cvt_ceil_full;
		}
		else {
			iw_errdiff_dither(ctx,ctx->img2_ci[channel].dithersubtype,d_floor,x,y);
			s_full=s_cvt_floor_full;
		}
	}
	else if(ditherfamily==IW_DITHERFAMILY_ORDERED) {
		dd=iw_ordered_dither(ctx->img2_ci[channel].dithersubtype, d_floor/(d_floor+d_ceil),x,y);
		s_full = dd ? s_cvt_ceil_full : s_cvt_floor_full;
	}
	else if(ditherfamily==IW_DITHERFAMILY_RANDOM) {
		dd=iw_random_dither(ctx,d_floor/(d_floor+d_ceil),x,y,ctx->img2_ci[channel].dithersubtype,channel);
		s_full = dd ? s_cvt_ceil_full : s_cvt_floor_full;
	}
	else {
		s_full = 0.0;
	}

okay:
	put_raw_sample(ctx,s_full,x,y,channel);
}
