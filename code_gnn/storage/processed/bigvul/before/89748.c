static void install_list_entry(const char *lib) {
	assert(lib);

	int len = strlen(lib);
	if (strcspn(lib, "\\&!?\"'<>%^(){}[];,") != (size_t)len ||
	strstr(lib, "..")) {
		fprintf(stderr, "Error: \"%s\" is an invalid library\n", lib);
		exit(1);
	}

	if (*lib == '/')
		return load_library(lib);


	int i;
	for (i = 0; default_lib_paths[i]; i++) {
		char *fname = NULL;
		if (asprintf(&fname, "%s/%s", default_lib_paths[i], lib) == -1)
			errExit("asprintf");

#define DO_GLOBBING
#ifdef DO_GLOBBING
		glob_t globbuf;
		int globerr = glob(fname, GLOB_NOCHECK | GLOB_NOSORT | GLOB_PERIOD, NULL, &globbuf);
		if (globerr) {
			fprintf(stderr, "Error: failed to glob private-lib pattern %s\n", fname);
			exit(1);
		}
		size_t j;
		for (j = 0; j < globbuf.gl_pathc; j++) {
			assert(globbuf.gl_pathv[j]);
			load_library(globbuf.gl_pathv[j]);
		}

		globfree(&globbuf);
#else
		load_library(fname);
#endif
		free(fname);
	}

	return;
}