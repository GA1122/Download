lseg_crossing(double x, double y, double prev_x, double prev_y)
{
	double		z;
	int			y_sign;

	if (FPzero(y))
	{							 
		if (FPzero(x))			 
			return POINT_ON_POLYGON;
		else if (FPgt(x, 0))
		{						 
			if (FPzero(prev_y))  
				 
				return FPgt(prev_x, 0) ? 0 : POINT_ON_POLYGON;
			return FPlt(prev_y, 0) ? 1 : -1;
		}
		else
		{						 
			if (FPzero(prev_y))
				 
				return FPlt(prev_x, 0) ? 0 : POINT_ON_POLYGON;
			return 0;
		}
	}
	else
	{							 
		 
		y_sign = FPgt(y, 0) ? 1 : -1;

		if (FPzero(prev_y))
			 
			return FPlt(prev_x, 0) ? 0 : y_sign;
		else if (FPgt(y_sign * prev_y, 0))
			 
			return 0;			 
		else
		{						 
			if (FPge(x, 0) && FPgt(prev_x, 0))
				 
				return 2 * y_sign;
			if (FPlt(x, 0) && FPle(prev_x, 0))
				 
				return 0;

			 
			z = (x - prev_x) * y - (y - prev_y) * x;
			if (FPzero(z))
				return POINT_ON_POLYGON;
			return FPgt((y_sign * z), 0) ? 0 : 2 * y_sign;
		}
	}
}
