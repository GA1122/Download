static int ok_to_run(const char *program) {
	if (strstr(program, "/")) {
		if (access(program, X_OK) == 0)  
			return 1;
	}
	else {  
		char *path1 = getenv("PATH");
		if (path1) {
			if (arg_debug)
				printf("Searching $PATH for %s\n", program);
			char *path2 = strdup(path1);
			if (!path2)
				errExit("strdup");

			char *ptr = strtok(path2, ":");
			while (ptr) {
				char *fname;

				if (asprintf(&fname, "%s/%s", ptr, program) == -1)
					errExit("asprintf");
				if (arg_debug)
					printf("trying #%s#\n", fname);

				struct stat s;
				int rv = stat(fname, &s);
				if (rv == 0) {
					if (access(fname, X_OK) == 0) {
						free(path2);
						free(fname);
						return 1;
					}
					else
						fprintf(stderr, "Error: execute permission denied for %s\n", fname);

					free(fname);
					break;
				}

				free(fname);
				ptr = strtok(NULL, ":");
			}
			free(path2);
		}
	}
	return 0;
}