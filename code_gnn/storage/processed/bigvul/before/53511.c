get_time_t_min(void)
{
#if defined(TIME_T_MIN)
	return TIME_T_MIN;
#else
	if (((time_t)0) < ((time_t)-1)) {
		 
		return (time_t)0;
	} else {
		 
		if (sizeof(time_t) == sizeof(int64_t)) {
			return (time_t)INT64_MIN;
		} else {
			return (time_t)INT32_MIN;
		}
	}
#endif
}
