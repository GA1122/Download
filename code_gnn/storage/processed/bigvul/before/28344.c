static int au1200fb_fb_blank(int blank_mode, struct fb_info *fbi)
{
	struct au1200fb_device *fbdev = fbi->par;

	 
	if (noblanking)
		return 0;

	switch (blank_mode) {

	case FB_BLANK_UNBLANK:
	case FB_BLANK_NORMAL:
		 
		au1200_setpanel(panel, fbdev->pd);
		break;
	case FB_BLANK_VSYNC_SUSPEND:
	case FB_BLANK_HSYNC_SUSPEND:
	case FB_BLANK_POWERDOWN:
		 
		au1200_setpanel(NULL, fbdev->pd);
		break;
	default:
		break;

	}

	 
	return (blank_mode == FB_BLANK_NORMAL) ? -EINVAL : 0;
}
