int setproctitle(char *title)
{
	char buf[2048], *tmp;
	FILE *f;
	int i, len, ret = 0;
	unsigned long arg_start, arg_end, env_start, env_end;

	f = fopen_cloexec("/proc/self/stat", "r");
	if (!f) {
		return -1;
	}

	tmp = fgets(buf, sizeof(buf), f);
	fclose(f);
	if (!tmp) {
		return -1;
	}

	 
	tmp = strchr(buf, ' ');
	for (i = 0; i < 46; i++) {
		if (!tmp)
			return -1;
		tmp = strchr(tmp+1, ' ');
	}

	if (!tmp)
		return -1;

	i = sscanf(tmp, "%lu %lu %lu %lu", &arg_start, &arg_end, &env_start, &env_end);
	if (i != 4) {
		return -1;
	}

	 
	len = strlen(title) + 1;

	 
	if (len > env_end - arg_start) {
		arg_end = env_end;
		len = env_end - arg_start;
	} else {
		 
		if (len >= arg_end - arg_start) {
			env_start = env_end;
		}

		arg_end = arg_start + len;

		 
		if (arg_end < len || arg_end < arg_start) {
			return -1;
		}

	}

	strcpy((char*)arg_start, title);

	ret |= prctl(PR_SET_MM, PR_SET_MM_ARG_START,   arg_start, 0, 0);
	ret |= prctl(PR_SET_MM, PR_SET_MM_ARG_END,     arg_end, 0, 0);
	ret |= prctl(PR_SET_MM, PR_SET_MM_ENV_START,   env_start, 0, 0);
	ret |= prctl(PR_SET_MM, PR_SET_MM_ENV_END,     env_end, 0, 0);

 	return ret;
 }
