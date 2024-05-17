int fb_show_logo(struct fb_info *info, int rotate)
{
	int y;

	y = fb_show_logo_line(info, rotate, fb_logo.logo, 0,
			      num_online_cpus());
	y = fb_show_extra_logos(info, y, rotate);

	return y;
}
