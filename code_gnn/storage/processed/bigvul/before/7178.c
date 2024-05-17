static unsigned constant_time_ge(unsigned a, unsigned b)
	{
	a -= b;
	return DUPLICATE_MSB_TO_ALL(~a);
	}
