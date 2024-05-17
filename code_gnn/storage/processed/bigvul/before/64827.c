static void iwjpg_set_density(struct iw_context *ctx,struct jpeg_compress_struct *cinfo,
	const struct iw_image *img)
{
	int pref_units;

	if(img->density_code==IW_DENSITY_UNITS_UNKNOWN) {
		cinfo->density_unit=0;  
		cinfo->X_density = (UINT16)(0.5+img->density_x);
		cinfo->Y_density = (UINT16)(0.5+img->density_y);
	}
	else if(img->density_code==IW_DENSITY_UNITS_PER_METER) {
		pref_units = iw_get_value(ctx,IW_VAL_PREF_UNITS);

		if(pref_units==IW_PREF_UNITS_METRIC) {
			cinfo->density_unit=2;  
			cinfo->X_density = (UINT16)(0.5+ img->density_x*0.01);
			cinfo->Y_density = (UINT16)(0.5+ img->density_y*0.01);
		}
		else {
			cinfo->density_unit=1;  
			cinfo->X_density = (UINT16)(0.5+ img->density_x*0.0254);
			cinfo->Y_density = (UINT16)(0.5+ img->density_y*0.0254);
		}
	}
}