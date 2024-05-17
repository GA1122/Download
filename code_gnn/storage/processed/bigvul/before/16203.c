GahpClient::gt4_gram_client_job_create(
	const char * submit_id,								   
	const char * resource_manager_contact,
	const char * jobmanager_type,
	const char * callback_contact,
	const char * rsl,
	time_t termination_time,
	char ** job_contact)
{

	static const char* command = "GT4_GRAM_JOB_SUBMIT";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if (!resource_manager_contact) resource_manager_contact=NULLSTRING;
	if (!rsl) rsl=NULLSTRING;
	if (!callback_contact) callback_contact=NULLSTRING;
	
	char * _submit_id = strdup (escapeGahpString(submit_id));
	char * _resource_manager_contact = 
		strdup (escapeGahpString(resource_manager_contact));
	char * _jobmanager_type = strdup (escapeGahpString(jobmanager_type));
	char * _callback_contact = strdup (escapeGahpString(callback_contact));
	char * _rsl = strdup (escapeGahpString(rsl));

	std::string reqline;
	int x = sprintf(reqline, "%s %s %s %s %s %d", 
							 _submit_id,
							 _resource_manager_contact,
							 _jobmanager_type,
							 _callback_contact,
							 _rsl,
							 (int)termination_time);


	free (_submit_id);
	free (_resource_manager_contact);
	free (_jobmanager_type);
	free (_callback_contact);
	free (_rsl);

	ASSERT( x > 0 );
	const char *buf = reqline.c_str();

	if ( !is_pending(command,buf) ) {
		if ( m_mode == results_only ) {
			return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
		}
		now_pending(command,buf,deleg_proxy);
	}

		
	Gahp_Args* result = get_pending_result(command,buf);
	if ( result ) {
		if (result->argc != 4) {
			EXCEPT("Bad %s Result",command);
		}
		int rc = atoi(result->argv[1]);
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			*job_contact = strdup(result->argv[2]);
		}
		if ( strcasecmp(result->argv[3], NULLSTRING) ) {
			error_string = result->argv[3];
		} else {
			error_string = "";
		}
		delete result;
		return rc;
	}

	if ( check_pending_timeout(command,buf) ) {
		sprintf( error_string, "%s timed out", command );
		return GAHPCLIENT_COMMAND_TIMED_OUT;
	}

	return GAHPCLIENT_COMMAND_PENDING;
}
