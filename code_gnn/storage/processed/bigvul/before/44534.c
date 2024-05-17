static void check_supports_multiple_controllers(pid_t pid)
{
	FILE *f;
	char *line = NULL, *prevpath = NULL;
	size_t sz = 0;
	char path[100];

	cgm_supports_multiple_controllers = false;
	cgm_all_controllers_same = false;

	if (api_version < CGM_SUPPORTS_MULT_CONTROLLERS) {
		cgm_supports_multiple_controllers = false;
		return;
	}

	cgm_supports_multiple_controllers = true;

	if (pid == -1)
		sprintf(path, "/proc/self/cgroup");
	else
		sprintf(path, "/proc/%d/cgroup", pid);
	f = fopen(path, "r");
	if (!f)
		return;

	cgm_all_controllers_same = true;

	while (getline(&line, &sz, f) != -1) {
		 
		char *colon;
		if (!line[0])
			continue;

		colon = strchr(line, ':');
		if (!colon)
			continue;
		colon = strchr(colon+1, ':');
		if (!colon)
			continue;
		colon++;
		if (!prevpath) {
			prevpath = alloca(strlen(colon)+1);
			strcpy(prevpath, colon);
			continue;
		}
		if (strcmp(prevpath, colon) != 0) {
			cgm_all_controllers_same = false;
			break;
		}
	}

	fclose(f);
	free(line);
}