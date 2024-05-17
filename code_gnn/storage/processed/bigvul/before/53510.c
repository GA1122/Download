get_time_t_max(void)
{
#if defined(TIME_T_MAX)
	return TIME_T_MAX;
#else
	 
	if (((time_t)0) < ((time_t)-1)) {
		 
		return (~(time_t)0);
	} else {
		 
		 
		if (sizeof(time_t) == sizeof(int64_t)) {
			return (time_t)INT64_MAX;
		} else {
			return (time_t)INT32_MAX;
		}
	}
#endif
}
