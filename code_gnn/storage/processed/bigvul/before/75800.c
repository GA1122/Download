init_global_data(data_t * data, data_t *old_global_data)
{
	 
	char unknown_name[] = "[unknown]";

	 
	if (old_global_data) {
		data->local_name = old_global_data->local_name;
		old_global_data->local_name = NULL;
	}

	if (!data->local_name &&
	    (!data->router_id ||
	     (data->smtp_server.ss_family &&
	      (!data->smtp_helo_name ||
	       !data->email_from)))) {
		data->local_name = get_local_name();

		 
		if (!data->local_name) {
			data->local_name = MALLOC(sizeof(unknown_name));
			strcpy(data->local_name, unknown_name);
		}
	}

	if (!data->router_id)
		set_default_router_id(data, data->local_name);

	if (data->smtp_server.ss_family) {
		if (!data->smtp_connection_to)
			set_default_smtp_connection_timeout(data);

		if (strcmp(data->local_name, unknown_name)) {
			if (!data->email_from)
				set_default_email_from(data, data->local_name);

			if (!data->smtp_helo_name)
				data->smtp_helo_name = data->local_name;
		}
	}

	 
#ifdef _WITH_VRRP_
	 
	if (
#ifndef _DEBUG_
	    prog_type == PROG_TYPE_VRRP &&
#endif
	    data->notify_fifo.name && data->vrrp_notify_fifo.name &&
	    !strcmp(data->notify_fifo.name, data->vrrp_notify_fifo.name)) {
		log_message(LOG_INFO, "notify FIFO %s has been specified for global and vrrp FIFO - ignoring vrrp FIFO", data->vrrp_notify_fifo.name);
		FREE_PTR(data->vrrp_notify_fifo.name);
		data->vrrp_notify_fifo.name = NULL;
		free_notify_script(&data->vrrp_notify_fifo.script);
	}
#endif
#ifdef _WITH_LVS_
	 
#ifndef _DEBUG_
	if (prog_type == PROG_TYPE_CHECKER)
#endif
	{
		if (data->notify_fifo.name && data->lvs_notify_fifo.name &&
		    !strcmp(data->notify_fifo.name, data->lvs_notify_fifo.name)) {
			log_message(LOG_INFO, "notify FIFO %s has been specified for global and LVS FIFO - ignoring LVS FIFO", data->lvs_notify_fifo.name);
			FREE_PTR(data->lvs_notify_fifo.name);
			data->lvs_notify_fifo.name = NULL;
			free_notify_script(&data->lvs_notify_fifo.script);
		}

#ifdef _WITH_VRRP_
		 
		if (data->lvs_notify_fifo.name && data->vrrp_notify_fifo.name &&
		    !strcmp(data->lvs_notify_fifo.name, data->vrrp_notify_fifo.name) &&
		    data->lvs_notify_fifo.script &&
		    data->vrrp_notify_fifo.script) {
			log_message(LOG_INFO, "LVS notify FIFO and vrrp FIFO are the same both with scripts - ignoring LVS FIFO script");
			free_notify_script(&data->lvs_notify_fifo.script);
		}
#endif
	}
#endif
}
