int log_service_states(int type, time_t *timestamp) {
	char *temp_buffer = NULL;
	service *temp_service = NULL;
	host *temp_host = NULL;;

	 
	if(type == INITIAL_STATES && log_initial_states == FALSE)
		return OK;

	for(temp_service = service_list; temp_service != NULL; temp_service = temp_service->next) {

		 
		if((temp_host = temp_service->host_ptr) == NULL)
			continue;

		asprintf(&temp_buffer, "%s SERVICE STATE: %s;%s;%s;%s;%d;%s\n",
				 (type == INITIAL_STATES) ? "INITIAL" : "CURRENT",
				 temp_service->host_name, temp_service->description,
				 service_state_name(temp_service->current_state),
				 state_type_name(temp_service->state_type),
				 temp_service->current_attempt,
				 temp_service->plugin_output);

		write_to_all_logs_with_timestamp(temp_buffer, NSLOG_INFO_MESSAGE, timestamp);

		my_free(temp_buffer);
		}

	return OK;
	}