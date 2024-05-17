int write_to_log(char *buffer, unsigned long data_type, time_t *timestamp) {
	FILE *fp;
	time_t log_time = 0L;

	if(buffer == NULL)
		return ERROR;

	 
	if(verify_config || test_scheduling == TRUE)
		return OK;

	 
	if(!(data_type & logging_options))
		return OK;

	fp = open_log_file();
	if (fp == NULL)
		return ERROR;
	 
	if(timestamp == NULL)
		time(&log_time);
	else
		log_time = *timestamp;

	 
	strip(buffer);

	 
	fprintf(fp, "[%llu] %s\n", (unsigned long long)log_time, buffer);
	fflush(fp);

#ifdef USE_EVENT_BROKER
	 
	broker_log_data(NEBTYPE_LOG_DATA, NEBFLAG_NONE, NEBATTR_NONE, buffer, data_type, log_time, NULL);
#endif

	return OK;
	}
