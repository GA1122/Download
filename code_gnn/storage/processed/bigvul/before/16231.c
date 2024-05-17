GahpClient::unicore_job_callback(unicore_gahp_callback_func_t callback_func)
{
	char buf[150];
	static const char* command = "UNICORE_JOB_CALLBACK";

	if ( server->unicore_gahp_callback_func != NULL ) {
		if ( callback_func != server->unicore_gahp_callback_func ) {
			EXCEPT("unicore_job_callback called twice");
		}
		return 0;
	}

	if  (server->m_commands_supported->contains_anycase(command)==FALSE) {
		return GAHPCLIENT_COMMAND_NOT_SUPPORTED;
	}

	if ( m_mode == results_only ) {
		return GAHPCLIENT_COMMAND_NOT_SUBMITTED;
	}

	int reqid = server->new_reqid();
	int x = snprintf(buf,sizeof(buf),"%s %d",command,reqid);
	ASSERT( x > 0 && x < (int)sizeof(buf) );
	server->write_line(buf);
	Gahp_Args result;
	server->read_argv(result);
	if ( result.argc != 1 || result.argv[0][0] != 'S' ) {
		dprintf(D_ALWAYS,"GAHP command '%s' failed\n",command);
		error_string = "";
		return 1;
	} 

	server->unicore_gahp_callback_reqid = reqid;
 	server->unicore_gahp_callback_func = callback_func;

	return 0;
}
