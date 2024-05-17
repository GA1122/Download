static void iwjpeg_read_density(struct iw_context *ctx, struct iw_image *img,
	struct jpeg_decompress_struct *cinfo)
{
	switch(cinfo->density_unit) {
	case 1:  
		img->density_x = ((double)cinfo->X_density)/0.0254;
		img->density_y = ((double)cinfo->Y_density)/0.0254;
		img->density_code = IW_DENSITY_UNITS_PER_METER;
		break;
	case 2:  
		img->density_x = ((double)cinfo->X_density)*100.0;
		img->density_y = ((double)cinfo->Y_density)*100.0;
		img->density_code = IW_DENSITY_UNITS_PER_METER;
		break;
	default:  
		if(cinfo->X_density!=cinfo->Y_density) {
			img->density_x = (double)cinfo->X_density;
			img->density_y = (double)cinfo->Y_density;
			img->density_code = IW_DENSITY_UNITS_UNKNOWN;
		}
	}
}
