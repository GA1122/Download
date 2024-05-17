static void clean_dir(const char *name, int *pidarr, int start_pid, int max_pids) {
	DIR *dir;
	if (!(dir = opendir(name))) {
		fwarning("cannot clean %s directory\n", name);
		return;  
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
		if (pidarr[pid] == 0)
			delete_run_files(pid);
	}
	closedir(dir);
}