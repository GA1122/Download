point_inside(Point *p, int npts, Point *plist)
{
	double		x0,
				y0;
	double		prev_x,
				prev_y;
	int			i = 0;
	double		x,
				y;
	int			cross,
				total_cross = 0;

	if (npts <= 0)
		return 0;

	 
	x0 = plist[0].x - p->x;
	y0 = plist[0].y - p->y;

	prev_x = x0;
	prev_y = y0;
	 
	for (i = 1; i < npts; i++)
	{
		 
		x = plist[i].x - p->x;
		y = plist[i].y - p->y;

		 
		if ((cross = lseg_crossing(x, y, prev_x, prev_y)) == POINT_ON_POLYGON)
			return 2;
		total_cross += cross;

		prev_x = x;
		prev_y = y;
	}

	 
	if ((cross = lseg_crossing(x0, y0, prev_x, prev_y)) == POINT_ON_POLYGON)
		return 2;
	total_cross += cross;

	if (total_cross != 0)
		return 1;
	return 0;
}
