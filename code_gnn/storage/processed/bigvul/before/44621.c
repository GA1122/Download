void remount_all_slave(void)
{
	 
	FILE *f = fopen("/proc/self/mountinfo", "r");
	char *line = NULL;
	size_t len = 0;

	if (!f) {
		SYSERROR("Failed to open /proc/self/mountinfo to mark all shared");
		ERROR("Continuing container startup...");
		return;
	}

	while (getline(&line, &len, f) != -1) {
		char *target, *opts;
		target = get_field(line, 4);
		if (!target)
			continue;
		opts = get_field(target, 2);
		if (!opts)
			continue;
		null_endofword(opts);
		if (!strstr(opts, "shared"))
			continue;
		null_endofword(target);
		if (mount(NULL, target, NULL, MS_SLAVE, NULL)) {
			SYSERROR("Failed to make %s rslave", target);
			ERROR("Continuing...");
		}
	}
	fclose(f);
	free(line);
}