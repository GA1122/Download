int log_debug_info(int level, int verbosity, const char *fmt, ...) {
	va_list ap;
	char *tmppath = NULL;
	struct timeval current_time;

	if(!(debug_level == DEBUGL_ALL || (level & debug_level)))
		return OK;

	if(verbosity > debug_verbosity)
		return OK;

	if(debug_file_fp == NULL)
		return ERROR;

	 
	gettimeofday(&current_time, NULL);
	fprintf(debug_file_fp, "[%lu.%06lu] [%03d.%d] [pid=%lu] ", current_time.tv_sec, current_time.tv_usec, level, verbosity, (unsigned long)getpid());

	 
	va_start(ap, fmt);
	vfprintf(debug_file_fp, fmt, ap);
	va_end(ap);

	 
	fflush(debug_file_fp);

	 
	if((unsigned long)ftell(debug_file_fp) > max_debug_file_size && max_debug_file_size > 0L) {

		 
		close_debug_log();

		 
		asprintf(&tmppath, "%s.old", debug_file);
		if(tmppath) {

			 
			unlink(tmppath);

			 
			my_rename(debug_file, tmppath);

			 
			my_free(tmppath);
			}

		 
		open_debug_log();
		}

	return OK;
	}
