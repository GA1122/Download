int fb_prepare_logo(struct fb_info *info, int rotate)
{
	int depth = fb_get_color_depth(&info->var, &info->fix);
	unsigned int yres;

	memset(&fb_logo, 0, sizeof(struct logo_data));

	if (info->flags & FBINFO_MISC_TILEBLITTING ||
	    info->flags & FBINFO_MODULE)
		return 0;

	if (info->fix.visual == FB_VISUAL_DIRECTCOLOR) {
		depth = info->var.blue.length;
		if (info->var.red.length < depth)
			depth = info->var.red.length;
		if (info->var.green.length < depth)
			depth = info->var.green.length;
	}

	if (info->fix.visual == FB_VISUAL_STATIC_PSEUDOCOLOR && depth > 4) {
		 
		depth = 4;
	}

	 
	fb_logo.logo = fb_find_logo(depth);

	if (!fb_logo.logo) {
		return 0;
	}

	if (rotate == FB_ROTATE_UR || rotate == FB_ROTATE_UD)
		yres = info->var.yres;
	else
		yres = info->var.xres;

	if (fb_logo.logo->height > yres) {
		fb_logo.logo = NULL;
		return 0;
	}

	 
	if (fb_logo.logo->type == LINUX_LOGO_CLUT224)
		fb_logo.depth = 8;
	else if (fb_logo.logo->type == LINUX_LOGO_VGA16)
		fb_logo.depth = 4;
	else
		fb_logo.depth = 1;


 	if (fb_logo.depth > 4 && depth > 4) {
 		switch (info->fix.visual) {
 		case FB_VISUAL_TRUECOLOR:
 			fb_logo.needs_truepalette = 1;
 			break;
 		case FB_VISUAL_DIRECTCOLOR:
 			fb_logo.needs_directpalette = 1;
 			fb_logo.needs_cmapreset = 1;
 			break;
 		case FB_VISUAL_PSEUDOCOLOR:
 			fb_logo.needs_cmapreset = 1;
 			break;
 		}
 	}

	return fb_prepare_extra_logos(info, fb_logo.logo->height, yres);
}
