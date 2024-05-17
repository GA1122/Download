static double filter_welsh(const double x)
{
	 
	if (x <  1.0)
		return(1 - x*x);
	return(0.0);
}
