int write_to_all_logs(char *buffer, unsigned long data_type) {

	 
	write_to_syslog(buffer, data_type);

	 
	write_to_log(buffer, data_type, NULL);

	return OK;
	}