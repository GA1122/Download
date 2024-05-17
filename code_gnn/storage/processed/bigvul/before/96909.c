static inline void set_cmdline(int idx, const char *cmdline)
{
	strncpy(get_saved_cmdlines(idx), cmdline, TASK_COMM_LEN);
}
