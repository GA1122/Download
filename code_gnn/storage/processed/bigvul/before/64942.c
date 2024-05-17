static iw_tmpsample linear_to_x_sample(iw_tmpsample samp_lin, const struct iw_csdescr *csdescr)
{
	if(samp_lin > 0.999999999) {
		return 1.0;
	}

	switch(csdescr->cstype) {
	case IW_CSTYPE_SRGB:
		return linear_to_srgb_sample(samp_lin);
	case IW_CSTYPE_LINEAR:
		return samp_lin;
	case IW_CSTYPE_GAMMA:
		return linear_to_gamma_sample(samp_lin,csdescr->gamma);
	case IW_CSTYPE_REC709:
		return linear_to_rec709_sample(samp_lin);
	}
	return linear_to_srgb_sample(samp_lin);
}