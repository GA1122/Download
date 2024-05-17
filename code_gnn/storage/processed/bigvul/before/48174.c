static void write_to_logs_and_console(char *buffer, unsigned long data_type, int display) {
	register int len = 0;
	register int x = 0;

	 
	len = strlen(buffer);
	for(x = len - 1; x >= 0; x--) {
		if(buffer[x] == '\n')
			buffer[x] = '\x0';
		else
			break;
		}

	 
	write_to_all_logs(buffer, data_type);

	 
	if(display == TRUE) {

		 
		if(test_scheduling == TRUE && data_type == NSLOG_VERIFICATION_WARNING)
			return;

		write_to_console(buffer);
		}
	}
