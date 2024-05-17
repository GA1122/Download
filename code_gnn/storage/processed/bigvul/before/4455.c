static int phar_analyze_path(const char *fname, const char *ext, int ext_len, int for_create TSRMLS_DC)  
{
	php_stream_statbuf ssb;
	char *realpath;
	char *filename = estrndup(fname, (ext - fname) + ext_len);

	if ((realpath = expand_filepath(filename, NULL TSRMLS_CC))) {
#ifdef PHP_WIN32
		phar_unixify_path_separators(realpath, strlen(realpath));
#endif
		if (zend_hash_exists(&(PHAR_GLOBALS->phar_fname_map), realpath, strlen(realpath))) {
			efree(realpath);
			efree(filename);
			return SUCCESS;
		}

		if (PHAR_G(manifest_cached) && zend_hash_exists(&cached_phars, realpath, strlen(realpath))) {
			efree(realpath);
			efree(filename);
			return SUCCESS;
		}
		efree(realpath);
	}

	if (SUCCESS == php_stream_stat_path((char *) filename, &ssb)) {

		efree(filename);

		if (ssb.sb.st_mode & S_IFDIR) {
			return FAILURE;
		}

		if (for_create == 1) {
			return FAILURE;
		}

		return SUCCESS;
	} else {
		char *slash;

		if (!for_create) {
			efree(filename);
			return FAILURE;
		}

		slash = (char *) strrchr(filename, '/');

		if (slash) {
			*slash = '\0';
		}

		if (SUCCESS != php_stream_stat_path((char *) filename, &ssb)) {
			if (!slash) {
				if (!(realpath = expand_filepath(filename, NULL TSRMLS_CC))) {
					efree(filename);
					return FAILURE;
				}
#ifdef PHP_WIN32
				phar_unixify_path_separators(realpath, strlen(realpath));
#endif
				slash = strstr(realpath, filename);
				if (slash) {
					slash += ((ext - fname) + ext_len);
					*slash = '\0';
				}
				slash = strrchr(realpath, '/');

				if (slash) {
					*slash = '\0';
				} else {
					efree(realpath);
					efree(filename);
					return FAILURE;
				}

				if (SUCCESS != php_stream_stat_path(realpath, &ssb)) {
					efree(realpath);
					efree(filename);
					return FAILURE;
				}

				efree(realpath);

				if (ssb.sb.st_mode & S_IFDIR) {
					efree(filename);
					return SUCCESS;
				}
			}

			efree(filename);
			return FAILURE;
		}

		efree(filename);

		if (ssb.sb.st_mode & S_IFDIR) {
			return SUCCESS;
		}

		return FAILURE;
	}
}
 