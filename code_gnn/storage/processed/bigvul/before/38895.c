line_construct_pm(Point *pt, double m)
{
	LINE	   *result = (LINE *) palloc(sizeof(LINE));

	if (m == DBL_MAX)
	{
		 
		result->A = -1;
		result->B = 0;
		result->C = pt->x;
	}
	else
	{
		 
		result->A = m;
		result->B = -1.0;
		result->C = pt->y - m * pt->x;
	}

	return result;
}
