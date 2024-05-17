void preproc_clean_run(void) {
	int max_pids=32769;
	int start_pid = 100;
	FILE *fp = fopen("/proc/sys/kernel/pid_max", "r");
	if (fp) {
		int val;
		if (fscanf(fp, "%d", &val) == 1) {
			if (val > 4194304)	 
				val = 4194304;
			if (val >= max_pids)
				max_pids = val + 1;
		}
		fclose(fp);
	}
	int *pidarr = malloc(max_pids * sizeof(int));
	if (!pidarr)
		errExit("malloc");

	memset(pidarr, 0, max_pids * sizeof(int));

	DIR *dir;
	if (!(dir = opendir("/proc"))) {
		sleep(2);
		if (!(dir = opendir("/proc"))) {
			fprintf(stderr, "Error: cannot open /proc directory\n");
			exit(1);
		}
	}

	struct dirent *entry;
	char *end;
	while ((entry = readdir(dir)) != NULL) {
		pid_t pid = strtol(entry->d_name, &end, 10);
		pid %= max_pids;
		if (end == entry->d_name || *end)
			continue;

		if (pid < start_pid)
			continue;
		pidarr[pid] = 1;
	}
	closedir(dir);

	clean_dir(RUN_FIREJAIL_PROFILE_DIR, pidarr, start_pid, max_pids);
	clean_dir(RUN_FIREJAIL_NAME_DIR, pidarr, start_pid, max_pids);

	free(pidarr);
}
