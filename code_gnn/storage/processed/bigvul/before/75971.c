 timeval_to_double(const timeval_t *t)
{
	 
	return (double)t->tv_sec + (double)t->tv_usec / TIMER_HZ_FLOAT;
}
