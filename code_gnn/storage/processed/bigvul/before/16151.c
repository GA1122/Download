GahpClient::condor_job_update_lease(const char *schedd_name,
									const SimpleList<PROC_ID> &jobs,
									const SimpleList<int> &expirations,
									SimpleList<PROC_ID> &updated )
{
	static const char* command = "CONDOR_JOB_UPDATE_LEASE";

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	ASSERT( jobs.Length() == expirations.Length() );

	if (!schedd_name) schedd_name=NULLSTRING;
	std::string reqline;
	char *esc1 = strdup( escapeGahpString(schedd_name) );
	int x = sprintf(reqline, "%s %d", esc1, jobs.Length());
	free( esc1 );
	ASSERT( x > 0 );
	SimpleListIterator<PROC_ID> jobs_i (jobs);
	SimpleListIterator<int> exps_i (expirations);
	PROC_ID next_job;
	int next_exp;
	while ( jobs_i.Next( next_job ) && exps_i.Next( next_exp ) ) {
		x = sprintf_cat( reqline, " %d.%d %d", next_job.cluster, next_job.proc,
								 next_exp );
		ASSERT( x > 0 );
	}
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
		int rc = 1;
		if ( result->argv[1][0] == 'S' ) {
			rc = 0;
		}
		if ( strcasecmp(result->argv[2], NULLSTRING) ) {
			error_string = result->argv[2];
		} else {
			error_string = "";
		}
		updated.Clear();
		char *ptr1 = result->argv[3];
		while ( ptr1 != NULL && *ptr1 != '\0' ) {
			int i;
			PROC_ID job_id;
			char *ptr2 = strchr( ptr1, ',' );
			if ( ptr2 ) {
				*ptr2 = '\0';
				ptr2++;
			}
			i = sscanf( ptr1, "%d.%d", &job_id.cluster, &job_id.proc );
			if ( i != 2 ) {
				dprintf( D_ALWAYS, "condor_job_update_lease: skipping malformed job id '%s'\n", ptr1 );
			} else {
				updated.Append( job_id );
			}
			ptr1 = ptr2;
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
