static int lxc_attach_set_environment(enum lxc_attach_env_policy_t policy, char** extra_env, char** extra_keep)
{
	if (policy == LXC_ATTACH_CLEAR_ENV) {
		char **extra_keep_store = NULL;
		int path_kept = 0;

		if (extra_keep) {
			size_t count, i;

			for (count = 0; extra_keep[count]; count++);

			extra_keep_store = calloc(count, sizeof(char *));
			if (!extra_keep_store) {
				SYSERROR("failed to allocate memory for storing current "
				         "environment variable values that will be kept");
				return -1;
			}
			for (i = 0; i < count; i++) {
				char *v = getenv(extra_keep[i]);
				if (v) {
					extra_keep_store[i] = strdup(v);
					if (!extra_keep_store[i]) {
						SYSERROR("failed to allocate memory for storing current "
						         "environment variable values that will be kept");
						while (i > 0)
							free(extra_keep_store[--i]);
						free(extra_keep_store);
						return -1;
					}
					if (strcmp(extra_keep[i], "PATH") == 0)
						path_kept = 1;
				}
				 
			}
		}

		if (clearenv()) {
			char **p;
			SYSERROR("failed to clear environment");
			if (extra_keep_store) {
				for (p = extra_keep_store; *p; p++)
					free(*p);
				free(extra_keep_store);
			}
			return -1;
		}

		if (extra_keep_store) {
			size_t i;
			for (i = 0; extra_keep[i]; i++) {
				if (extra_keep_store[i]) {
					if (setenv(extra_keep[i], extra_keep_store[i], 1) < 0)
						SYSERROR("Unable to set environment variable");
				}
				free(extra_keep_store[i]);
			}
			free(extra_keep_store);
		}

		 
		if (!path_kept)
			setenv("PATH", "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", 1);
	}

	if (putenv("container=lxc")) {
		SYSERROR("failed to set environment variable");
		return -1;
	}

	 
	if (extra_env) {
		for (; *extra_env; extra_env++) {
			 
			char *p = strdup(*extra_env);
			 
			if (!p) {
				SYSERROR("failed to allocate memory for additional environment "
				         "variables");
				return -1;
			}
			putenv(p);
		}
	}

	return 0;
}