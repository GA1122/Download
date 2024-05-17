static double filter_sinc(const double x)
{
	 
	if (x == 0.0) return(1.0);
	return (sin(M_PI * (double) x) / (M_PI * (double) x));
}
