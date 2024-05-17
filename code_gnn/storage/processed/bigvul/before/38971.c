pg_hypot(double x, double y)
{
	double		yx;

	 
	if (isinf(x) || isinf(y))
		return get_float8_infinity();

	if (isnan(x) || isnan(y))
		return get_float8_nan();

	 
	x = fabs(x);
	y = fabs(y);

	 
	if (x < y)
	{
		double		temp = x;

		x = y;
		y = temp;
	}

	 
	if (y == 0.0)
		return x;

	 
	yx = y / x;
	return x * sqrt(1.0 + (yx * yx));
}
