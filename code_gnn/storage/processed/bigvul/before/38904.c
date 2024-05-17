line_interpt_internal(LINE *l1, LINE *l2)
{
	Point	   *result;
	double		x,
				y;

	 
	if (DatumGetBool(DirectFunctionCall2(line_parallel,
										 LinePGetDatum(l1),
										 LinePGetDatum(l2))))
		return NULL;

	if (FPzero(l1->B))			 
	{
		x = l1->C;
		y = (l2->A * x + l2->C);
	}
	else if (FPzero(l2->B))		 
	{
		x = l2->C;
		y = (l1->A * x + l1->C);
	}
	else
	{
		x = (l1->C - l2->C) / (l2->A - l1->A);
		y = (l1->A * x + l1->C);
	}
	result = point_construct(x, y);

#ifdef GEODEBUG
	printf("line_interpt- lines are A=%.*g, B=%.*g, C=%.*g, A=%.*g, B=%.*g, C=%.*g\n",
		   DBL_DIG, l1->A, DBL_DIG, l1->B, DBL_DIG, l1->C, DBL_DIG, l2->A, DBL_DIG, l2->B, DBL_DIG, l2->C);
	printf("line_interpt- lines intersect at (%.*g,%.*g)\n", DBL_DIG, x, DBL_DIG, y);
#endif

	return result;
}
