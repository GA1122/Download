static int get_nearest_valid_colors(struct iw_context *ctx, iw_tmpsample samp_lin,
		const struct iw_csdescr *csdescr,
		double *s_lin_floor_1, double *s_lin_ceil_1,
		double *s_cvt_floor_full, double *s_cvt_ceil_full,
		double overall_maxcolorcode, int color_count)
{
	iw_tmpsample samp_cvt;
	double samp_cvt_expanded;
	unsigned int floor_int, ceil_int;

	samp_cvt = linear_to_x_sample(samp_lin,csdescr);

	if(color_count==0) {
		samp_cvt_expanded = samp_cvt * overall_maxcolorcode;
		if(samp_cvt_expanded>overall_maxcolorcode) samp_cvt_expanded=overall_maxcolorcode;
		if(samp_cvt_expanded<0.0) samp_cvt_expanded=0.0;

		*s_cvt_floor_full = floor(samp_cvt_expanded);
		*s_cvt_ceil_full  = ceil(samp_cvt_expanded);
	}
	else {
		double posterized_maxcolorcode;

		posterized_maxcolorcode = (double)(color_count-1);

		samp_cvt_expanded = samp_cvt * posterized_maxcolorcode;
		if(samp_cvt_expanded>posterized_maxcolorcode) samp_cvt_expanded=posterized_maxcolorcode;
		if(samp_cvt_expanded<0.0) samp_cvt_expanded=0.0;

		*s_cvt_floor_full = floor(0.5000000001 + floor(samp_cvt_expanded) * (overall_maxcolorcode/posterized_maxcolorcode));
		*s_cvt_ceil_full  = floor(0.5000000001 + ceil (samp_cvt_expanded) * (overall_maxcolorcode/posterized_maxcolorcode));
	}

	floor_int = (unsigned int)(*s_cvt_floor_full);
	ceil_int  = (unsigned int)(*s_cvt_ceil_full);
	if(floor_int == ceil_int) {
		return 1;
	}

	*s_lin_floor_1 = cvt_int_sample_to_linear_output(ctx,floor_int,csdescr,overall_maxcolorcode);
	*s_lin_ceil_1 =  cvt_int_sample_to_linear_output(ctx,ceil_int ,csdescr,overall_maxcolorcode);

	return 0;
}
