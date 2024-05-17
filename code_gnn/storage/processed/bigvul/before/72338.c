check_parent_exists(void)
{
	 
	if (parent_pid != -1 && getppid() != parent_pid) {
		 
		cleanup_socket();
		_exit(2);
	}
}
