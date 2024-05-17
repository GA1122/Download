static IW_INLINE iw_tmpsample linear_to_rec709_sample(iw_tmpsample v_linear)
{
	if(v_linear < 0.020) {
		return 4.5*v_linear;
	}
	return 1.099*pow(v_linear,0.45) - 0.099;
}
