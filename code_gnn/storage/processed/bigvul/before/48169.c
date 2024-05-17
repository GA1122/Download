int write_log_file_info(time_t *timestamp) {
	char *temp_buffer = NULL;

	 
	asprintf(&temp_buffer, "LOG VERSION: %s\n", LOG_VERSION_2);
	write_to_all_logs_with_timestamp(temp_buffer, NSLOG_PROCESS_INFO, timestamp);
	my_free(temp_buffer);

	return OK;
	}