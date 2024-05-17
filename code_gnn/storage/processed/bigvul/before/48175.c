int write_to_syslog(char *buffer, unsigned long data_type) {

	if(buffer == NULL)
		return ERROR;

	 
	if(verify_config || test_scheduling == TRUE)
		return OK;

	 
	if(use_syslog == FALSE)
		return OK;

	 
	if(!(data_type & syslog_options))
		return OK;

	 
	syslog(LOG_USER | LOG_INFO, "%s", buffer);

	return OK;
	}
