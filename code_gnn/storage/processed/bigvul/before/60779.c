SYSCALL_DEFINE1(exit_group, int, error_code)
{
	do_group_exit((error_code & 0xff) << 8);
	 
	return 0;
}