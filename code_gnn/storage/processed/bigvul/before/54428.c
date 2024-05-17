int php_zip_glob(char *pattern, int pattern_len, zend_long flags, zval *return_value)  
{
#ifdef HAVE_GLOB
	char cwd[MAXPATHLEN];
	int cwd_skip = 0;
#ifdef ZTS
	char work_pattern[MAXPATHLEN];
	char *result;
#endif
	glob_t globbuf;
	int n;
	int ret;

	if (pattern_len >= MAXPATHLEN) {
		php_error_docref(NULL, E_WARNING, "Pattern exceeds the maximum allowed length of %d characters", MAXPATHLEN);
		return -1;
	}

	if ((GLOB_AVAILABLE_FLAGS & flags) != flags) {
		php_error_docref(NULL, E_WARNING, "At least one of the passed flags is invalid or not supported on this platform");
		return -1;
	}

#ifdef ZTS
	if (!IS_ABSOLUTE_PATH(pattern, pattern_len)) {
		result = VCWD_GETCWD(cwd, MAXPATHLEN);
		if (!result) {
			cwd[0] = '\0';
		}
#ifdef PHP_WIN32
		if (IS_SLASH(*pattern)) {
			cwd[2] = '\0';
		}
#endif
		cwd_skip = strlen(cwd)+1;

		snprintf(work_pattern, MAXPATHLEN, "%s%c%s", cwd, DEFAULT_SLASH, pattern);
		pattern = work_pattern;
	}
#endif

	globbuf.gl_offs = 0;
	if (0 != (ret = glob(pattern, flags & GLOB_FLAGMASK, NULL, &globbuf))) {
#ifdef GLOB_NOMATCH
		if (GLOB_NOMATCH == ret) {
			 
			array_init(return_value);
			return 0;
		}
#endif
		return 0;
	}

	 
	if (!globbuf.gl_pathc || !globbuf.gl_pathv) {
		array_init(return_value);
		return 0;
	}

	 
	strncpy(cwd, globbuf.gl_pathv[0], MAXPATHLEN);
	if (ZIP_OPENBASEDIR_CHECKPATH(cwd)) {
		return -1;
	}

	array_init(return_value);
	for (n = 0; n < globbuf.gl_pathc; n++) {
		 
		if (flags & GLOB_ONLYDIR) {
			zend_stat_t s;

			if (0 != VCWD_STAT(globbuf.gl_pathv[n], &s)) {
				continue;
			}

			if (S_IFDIR != (s.st_mode & S_IFMT)) {
				continue;
			}
		}
		add_next_index_string(return_value, globbuf.gl_pathv[n]+cwd_skip);
	}

	globfree(&globbuf);
	return globbuf.gl_pathc;
#else
	php_error_docref(NULL, E_ERROR, "Glob support is not available");
	return 0;
#endif   
}
 
