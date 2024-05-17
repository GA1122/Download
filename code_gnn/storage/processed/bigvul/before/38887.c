dist_ps_internal(Point *pt, LSEG *lseg)
{
	double		m;				 
	LINE	   *ln;
	double		result,
				tmpdist;
	Point	   *ip;

	 
	if (lseg->p[1].x == lseg->p[0].x)
		m = 0;
	else if (lseg->p[1].y == lseg->p[0].y)
		m = (double) DBL_MAX;	 
	else
		m = (lseg->p[0].x - lseg->p[1].x) / (lseg->p[1].y - lseg->p[0].y);
	ln = line_construct_pm(pt, m);

#ifdef GEODEBUG
	printf("dist_ps- line is A=%g B=%g C=%g from (point) slope (%f,%f) %g\n",
		   ln->A, ln->B, ln->C, pt->x, pt->y, m);
#endif

	 

	 
	if ((ip = interpt_sl(lseg, ln)) != NULL)
	{
		 
		result = point_dt(pt, ip);
#ifdef GEODEBUG
		printf("dist_ps- distance is %f to intersection point is (%f,%f)\n",
			   result, ip->x, ip->y);
#endif
	}
	else
	{
		 
		result = point_dt(pt, &lseg->p[0]);
		tmpdist = point_dt(pt, &lseg->p[1]);
		if (tmpdist < result)
			result = tmpdist;
	}

	return result;
}
