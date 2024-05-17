static inline int cm_convert_to_ms(int iba_time)
{
	 
	return 1 << max(iba_time - 8, 0);
}
