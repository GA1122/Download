void bandwidth_remove(pid_t pid, const char *dev) {
	bandwidth_create_run_file(pid);
	
	read_bandwidth_file(pid);
	
	IFBW *elem = ifbw_find(dev);
	if (elem) {
		ifbw_remove(elem);
		write_bandwidth_file(pid) ;
	}
	
	if (ifbw == NULL) {
		 bandwidth_del_run_file(pid);
	}
}
