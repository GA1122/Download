static void handle_exif_density(struct iwjpegrcontext *rctx, struct iw_image *img)
{
	if(img->density_code!=IW_DENSITY_UNKNOWN) {
		return;
	}

	if(rctx->exif_density_x<=0.0 || rctx->exif_density_y<=0.0) return;

	switch(rctx->exif_density_unit) {
	case 1:  
		if(fabs(rctx->exif_density_x-rctx->exif_density_y)<0.00001)
			return;  
		img->density_x = rctx->exif_density_x;
		img->density_y = rctx->exif_density_y;
		img->density_code = IW_DENSITY_UNITS_UNKNOWN;
		break;
	case 2:  
		img->density_x = rctx->exif_density_x/0.0254;
		img->density_y = rctx->exif_density_y/0.0254;
		img->density_code = IW_DENSITY_UNITS_PER_METER;
		break;
	case 3:  
		img->density_x = rctx->exif_density_x*100.0;
		img->density_y = rctx->exif_density_y*100.0;
		img->density_code = IW_DENSITY_UNITS_PER_METER;
		break;
	}
}
