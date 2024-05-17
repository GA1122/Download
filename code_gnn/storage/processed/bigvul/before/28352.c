static void au1200fb_update_fbinfo(struct fb_info *fbi)
{
	 

	 
	if (panel_is_color(panel)) {
		if (fbi->var.bits_per_pixel <= 8) {
			 
			fbi->fix.visual = FB_VISUAL_PSEUDOCOLOR;
			fbi->fix.line_length = fbi->var.xres_virtual /
				(8/fbi->var.bits_per_pixel);
		} else {
			 
			fbi->fix.visual = FB_VISUAL_TRUECOLOR;
			fbi->fix.line_length = fbi->var.xres_virtual * (fbi->var.bits_per_pixel / 8);
		}
	} else {
		 
		fbi->fix.visual = FB_VISUAL_MONO10;
		fbi->fix.line_length = fbi->var.xres_virtual / 8;
	}

	fbi->screen_size = fbi->fix.line_length * fbi->var.yres_virtual;
	print_dbg("line length: %d\n", fbi->fix.line_length);
	print_dbg("bits_per_pixel: %d\n", fbi->var.bits_per_pixel);
}
