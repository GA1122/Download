int ms_tdiff(struct timeval *end, struct timeval *start)
{
	 
	if (unlikely(end->tv_sec - start->tv_sec > 3600))
		return 3600000;
	return (end->tv_sec - start->tv_sec) * 1000 + (end->tv_usec - start->tv_usec) / 1000;
}