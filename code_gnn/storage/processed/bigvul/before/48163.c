int log_host_event(host *hst) {
	char *temp_buffer = NULL;
	unsigned long log_options = 0L;

	 
	if(hst->current_state == HOST_DOWN)
		log_options = NSLOG_HOST_DOWN;
	else if(hst->current_state == HOST_UNREACHABLE)
		log_options = NSLOG_HOST_UNREACHABLE;
	else
		log_options = NSLOG_HOST_UP;

	asprintf(&temp_buffer, "HOST ALERT: %s;%s;%s;%d;%s\n",
			 hst->name,
			 host_state_name(hst->current_state),
			 state_type_name(hst->state_type),
			 hst->current_attempt,
			 (hst->plugin_output == NULL) ? "" : hst->plugin_output);

	write_to_all_logs(temp_buffer, log_options);

	my_free(temp_buffer);

	return OK;
	}