static void report_duplication(const char *full_path) {
	char *fname = strrchr(full_path, '/');
	if (fname && *(++fname) != '\0') {
		int i = 0;
		while (default_lib_paths[i]) {
			char *p;
			if (asprintf(&p, "%s/%s", default_lib_paths[i], fname) == -1)
				errExit("asprintf");
			fs_logger2("clone", p);
			free(p);
			i++;
		}
	}
}