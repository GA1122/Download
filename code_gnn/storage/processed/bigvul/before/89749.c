static void load_library(const char *fname) {
	assert(fname);
	assert(*fname == '/');

	struct stat s;
	if (stat(fname, &s) == 0 && s.st_uid == 0 && !access(fname, R_OK)) {
		if (is_dir(fname) || is_lib_64(fname)) {
			if (is_dir(fname))
				fslib_copy_dir(fname);
			else {
				if (strstr(fname, ".so") ||
				    access(fname, X_OK) != 0)  
					fslib_duplicate(fname);

				fslib_copy_libs(fname);
			}
		}
	}
}