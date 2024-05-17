int php_zip_pcre(zend_string *regexp, char *path, int path_len, zval *return_value)  
{
#ifdef ZTS
	char cwd[MAXPATHLEN];
	int cwd_skip = 0;
	char work_path[MAXPATHLEN];
	char *result;
#endif
	int files_cnt;
	zend_string **namelist;

#ifdef ZTS
	if (!IS_ABSOLUTE_PATH(path, path_len)) {
		result = VCWD_GETCWD(cwd, MAXPATHLEN);
		if (!result) {
			cwd[0] = '\0';
		}
#ifdef PHP_WIN32
		if (IS_SLASH(*path)) {
			cwd[2] = '\0';
		}
#endif
		cwd_skip = strlen(cwd)+1;

		snprintf(work_path, MAXPATHLEN, "%s%c%s", cwd, DEFAULT_SLASH, path);
		path = work_path;
	}
#endif

	if (ZIP_OPENBASEDIR_CHECKPATH(path)) {
		return -1;
	}

	files_cnt = php_stream_scandir(path, &namelist, NULL, (void *) php_stream_dirent_alphasort);

	if (files_cnt > 0) {
		pcre *re = NULL;
		pcre_extra *pcre_extra = NULL;
		int preg_options = 0, i;

		re = pcre_get_compiled_regex(regexp, &pcre_extra, &preg_options);
		if (!re) {
			php_error_docref(NULL, E_WARNING, "Invalid expression");
			return -1;
		}

		array_init(return_value);

		 
		for (i = 0; i < files_cnt; i++) {
			zend_stat_t s;
			char   fullpath[MAXPATHLEN];
			int    ovector[3];
			int    matches;
			int    namelist_len = ZSTR_LEN(namelist[i]);

			if ((namelist_len == 1 && ZSTR_VAL(namelist[i])[0] == '.') ||
				(namelist_len == 2 && ZSTR_VAL(namelist[i])[0] == '.' && ZSTR_VAL(namelist[i])[1] == '.')) {
				zend_string_release(namelist[i]);
				continue;
			}

			if ((path_len + namelist_len + 1) >= MAXPATHLEN) {
				php_error_docref(NULL, E_WARNING, "add_path string too long (max: %i, %i given)",
						MAXPATHLEN - 1, (path_len + namelist_len + 1));
				zend_string_release(namelist[i]);
				break;
			}

			snprintf(fullpath, MAXPATHLEN, "%s%c%s", path, DEFAULT_SLASH, ZSTR_VAL(namelist[i]));

			if (0 != VCWD_STAT(fullpath, &s)) {
				php_error_docref(NULL, E_WARNING, "Cannot read <%s>", fullpath);
				zend_string_release(namelist[i]);
				continue;
			}

			if (S_IFDIR == (s.st_mode & S_IFMT)) {
				zend_string_release(namelist[i]);
				continue;
			}

			matches = pcre_exec(re, NULL, ZSTR_VAL(namelist[i]), ZSTR_LEN(namelist[i]), 0, 0, ovector, 3);
			 
			if (matches < 0) {
				zend_string_release(namelist[i]);
				continue;
			}

			add_next_index_string(return_value, fullpath);
			zend_string_release(namelist[i]);
		}
		efree(namelist);
	}
	return files_cnt;
}
 
