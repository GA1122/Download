static int au1200fb_fb_check_var(struct fb_var_screeninfo *var,
	struct fb_info *fbi)
{
	struct au1200fb_device *fbdev = fbi->par;
	u32 pixclock;
	int screen_size, plane;

	plane = fbdev->plane;

	 
	var->xres = win->w[plane].xres;
	var->yres = win->w[plane].yres;

	 
	var->xres_virtual = var->xres;
	var->yres_virtual = var->yres;

	var->bits_per_pixel = winbpp(win->w[plane].mode_winctrl1);

	screen_size = var->xres_virtual * var->yres_virtual;
	if (var->bits_per_pixel > 8) screen_size *= (var->bits_per_pixel / 8);
	else screen_size /= (8/var->bits_per_pixel);

	if (fbdev->fb_len < screen_size)
		return -EINVAL;  

	 
	 
	pixclock = max((u32)(PICOS2KHZ(var->pixclock) * 1000), fbi->monspecs.dclkmin);
	pixclock = min3(pixclock, fbi->monspecs.dclkmax, (u32)AU1200_LCD_MAX_CLK/2);

	if (AU1200_LCD_MAX_CLK % pixclock) {
		int diff = AU1200_LCD_MAX_CLK % pixclock;
		pixclock -= diff;
	}

	var->pixclock = KHZ2PICOS(pixclock/1000);
#if 0
	if (!panel_is_active(panel)) {
		int pcd = AU1200_LCD_MAX_CLK / (pixclock * 2) - 1;

		if (!panel_is_color(panel)
			&& (panel->control_base & LCD_CONTROL_MPI) && (pcd < 3)) {
			 
			var->pixclock = KHZ2PICOS(6000);
		} else if (!pcd) {
			 
			var->pixclock = KHZ2PICOS(12000);
		}
	}
#endif
	 
	switch (var->bits_per_pixel) {
		case 16:
		{
			 
			int idx;
			idx = (win->w[0].mode_winctrl1 & LCD_WINCTRL1_FRM) >> 25;
			var->red    = rgb_bitfields[idx][0];
			var->green  = rgb_bitfields[idx][1];
			var->blue   = rgb_bitfields[idx][2];
			var->transp = rgb_bitfields[idx][3];
			break;
		}

		case 32:
		{
			 
			int idx;
			idx = (win->w[0].mode_winctrl1 & LCD_WINCTRL1_FRM) >> 25;
			var->red    = rgb_bitfields[idx][0];
			var->green  = rgb_bitfields[idx][1];
			var->blue   = rgb_bitfields[idx][2];
			var->transp = rgb_bitfields[idx][3];
			break;
		}
		default:
			print_dbg("Unsupported depth %dbpp", var->bits_per_pixel);
			return -EINVAL;
	}

	return 0;
}
