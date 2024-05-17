SYSCALL_DEFINE1(set_thread_area, struct user_desc __user *, u_info)
{
	return do_set_thread_area(current, -1, u_info, 1);
}
