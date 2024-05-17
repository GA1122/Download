int log_service_event(service *svc) {
	char *temp_buffer = NULL;
	unsigned long log_options = 0L;
	host *temp_host = NULL;

	 
	if(svc->state_type == SOFT_STATE && !log_service_retries)
		return OK;

	 
	if(svc->current_state == STATE_UNKNOWN)
		log_options = NSLOG_SERVICE_UNKNOWN;
	else if(svc->current_state == STATE_WARNING)
		log_options = NSLOG_SERVICE_WARNING;
	else if(svc->current_state == STATE_CRITICAL)
		log_options = NSLOG_SERVICE_CRITICAL;
	else
		log_options = NSLOG_SERVICE_OK;

	 
	if((temp_host = svc->host_ptr) == NULL)
		return ERROR;

	asprintf(&temp_buffer, "SERVICE ALERT: %s;%s;%s;%s;%d;%s\n",
			 svc->host_name, svc->description,
			 service_state_name(svc->current_state),
			 state_type_name(svc->state_type),
			 svc->current_attempt,
			 (svc->plugin_output == NULL) ? "" : svc->plugin_output);

	write_to_all_logs(temp_buffer, log_options);
	free(temp_buffer);

	return OK;
	}
