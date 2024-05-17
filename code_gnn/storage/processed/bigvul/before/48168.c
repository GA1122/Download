int rotate_log_file(time_t rotation_time) {
	char *temp_buffer = NULL;
	char method_string[16] = "";
	char *log_archive = NULL;
	struct tm *t, tm_s;
	int rename_result = 0;
	int stat_result = -1;
	struct stat log_file_stat;
	struct stat archive_stat;
	int archive_stat_result;

	if(log_rotation_method == LOG_ROTATION_NONE) {
		return OK;
		}
	else if(log_rotation_method == LOG_ROTATION_HOURLY)
		strcpy(method_string, "HOURLY");
	else if(log_rotation_method == LOG_ROTATION_DAILY)
		strcpy(method_string, "DAILY");
	else if(log_rotation_method == LOG_ROTATION_WEEKLY)
		strcpy(method_string, "WEEKLY");
	else if(log_rotation_method == LOG_ROTATION_MONTHLY)
		strcpy(method_string, "MONTHLY");
	else
		return ERROR;

	 
	last_log_rotation = time(NULL);
	update_program_status(FALSE);

	t = localtime_r(&rotation_time, &tm_s);

	stat_result = stat(log_file, &log_file_stat);

	close_log_file();

	 
	asprintf(&log_archive, "%s%snagios-%02d-%02d-%d-%02d.log", log_archive_path, (log_archive_path[strlen(log_archive_path) - 1] == '/') ? "" : "/", t->tm_mon + 1, t->tm_mday, t->tm_year + 1900, t->tm_hour);

	 
	archive_stat_result = stat(log_archive, &archive_stat);
	if((0 == archive_stat_result) || 
			((-1 == archive_stat_result) && (ENOENT != errno))) {
		return OK;
	}

	 
	rename_result = my_rename(log_file, log_archive);
	log_fp = open_log_file();
	if (log_fp == NULL)
		return ERROR;

	if(rename_result) {
		my_free(log_archive);
		return ERROR;
		}

	 
	asprintf(&temp_buffer, "LOG ROTATION: %s\n", method_string);
	write_to_all_logs_with_timestamp(temp_buffer, NSLOG_PROCESS_INFO, &rotation_time);
	my_free(temp_buffer);

	 
	write_log_file_info(&rotation_time);

	if(stat_result == 0) {
		chmod(log_file, log_file_stat.st_mode);
		chown(log_file, log_file_stat.st_uid, log_file_stat.st_gid);
		}

	 
	if(log_current_states==TRUE) {
		log_host_states(CURRENT_STATES, &rotation_time);
		log_service_states(CURRENT_STATES, &rotation_time);
	}

	 
	my_free(log_archive);

	return OK;
	}
