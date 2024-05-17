static ALWAYS_INLINE double SQRT(double X)
{
	 
	if (sizeof(float) != 4)
		return sqrt(X);

	 
	return my_SQRT(X);
}
