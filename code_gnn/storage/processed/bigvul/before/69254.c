void bandwidth_set(pid_t pid, const char *dev, int down, int up) {
	bandwidth_create_run_file(pid);

	char *txt;
	if (asprintf(&txt, "%s: RX %dKB/s, TX %dKB/s", dev, down, up) == -1)
		errExit("asprintf");
	
	read_bandwidth_file(pid);

	IFBW *ptr = ifbw_find(dev);
	if (ptr) {
		assert(ptr->txt);
		free(ptr->txt);
		ptr->txt = txt;
	}
	else {
		IFBW *ifbw_new = malloc(sizeof(IFBW));
		if (!ifbw_new)
			errExit("malloc");
		memset(ifbw_new, 0, sizeof(IFBW));
		ifbw_new->txt = txt;
		
		ifbw_add(ifbw_new);
	}
	write_bandwidth_file(pid) ;
}
