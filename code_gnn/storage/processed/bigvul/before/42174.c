static inline int current_is_64bit(void)
{
	 
	return !test_ti_thread_flag(task_thread_info(current), TIF_32BIT);
}
