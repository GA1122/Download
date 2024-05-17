static void write_to_all_logs_with_timestamp(char *buffer, unsigned long data_type, time_t *timestamp) {
	 
	write_to_syslog(buffer, data_type);

	 
	write_to_log(buffer, data_type, timestamp);
	}
