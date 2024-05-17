static int test_configured_paths(void)
{
	FILE *fp;
	nagios_macros *mac;

	mac = get_global_macros();

	fp = fopen(log_file, "a+");
	if (!fp) {
		 
		char *value_absolute = log_file;
		log_file = mac->x[MACRO_LOGFILE];
		logit(NSLOG_CONFIG_ERROR, TRUE, "Error: Failed to open logfile '%s' for writing: %s\n", value_absolute, strerror(errno));
		return ERROR;
		}

	fclose(fp);

	 
	mac->x[MACRO_LOGFILE] = log_file;
	return OK;
}