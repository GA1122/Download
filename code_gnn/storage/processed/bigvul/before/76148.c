bool check_conf_file(const char *conf_file)
{
	glob_t globbuf;
	size_t i;
	bool ret = true;
	int res;
	struct stat stb;
	unsigned num_matches = 0;

	globbuf.gl_offs = 0;
	res = glob(conf_file, GLOB_MARK
#if HAVE_DECL_GLOB_BRACE
					| GLOB_BRACE
#endif
						    , NULL, &globbuf);
	if (res) {
		report_config_error(CONFIG_FILE_NOT_FOUND, "Unable to find configuration file %s (glob returned %d)", conf_file, res);
		return false;
	}

	for (i = 0; i < globbuf.gl_pathc; i++) {
		if (globbuf.gl_pathv[i][strlen(globbuf.gl_pathv[i])-1] == '/') {
			 
			continue;
		}

		if (access(globbuf.gl_pathv[i], R_OK)) {
			log_message(LOG_INFO, "Unable to read configuration file %s", globbuf.gl_pathv[i]);
			ret = false;
			break;
		}

		 
		if (stat(globbuf.gl_pathv[i], &stb) ||
		    !S_ISREG(stb.st_mode) ||
		     (stb.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
			log_message(LOG_INFO, "Configuration file '%s' is not a regular non-executable file", globbuf.gl_pathv[i]);
			ret = false;
			break;
		}

		num_matches++;
	}

	if (ret) {
		if (num_matches > 1)
			report_config_error(CONFIG_MULTIPLE_FILES, "WARNING, more than one file matches configuration file %s, using %s", conf_file, globbuf.gl_pathv[0]);
		else if (num_matches == 0) {
			report_config_error(CONFIG_FILE_NOT_FOUND, "Unable to find configuration file %s", conf_file);
			ret = false;
		}
	}

	globfree(&globbuf);

	return ret;
}