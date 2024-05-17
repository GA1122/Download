GahpClient::now_pending(const char *command,const char *buf,
						GahpProxyInfo *cmd_proxy, PrioLevel prio_level )
{

	if ( command ) {
		clear_pending();
		pending_command = strdup( command );
		pending_reqid = server->new_reqid();
		if (buf) {
			pending_args = strdup(buf);
		}
		if (m_timeout) {
			pending_timeout = m_timeout;
		}
		pending_proxy = cmd_proxy;
		server->requestTable->insert(pending_reqid,this);
	}
	ASSERT( pending_command != NULL );

	if ( server->num_pending_requests >= server->max_pending_requests ) {
		switch ( prio_level ) {
		case high_prio:
			server->waitingHighPrio.push( pending_reqid );
			break;
		case medium_prio:
			server->waitingMediumPrio.push( pending_reqid );
			break;
		case low_prio:
			server->waitingLowPrio.push( pending_reqid );
			break;
		}
		return;
	}

	if ( server->is_initialized == true && server->can_cache_proxies == true ) {
		if ( server->useCachedProxy( pending_proxy ) != true ) {
			EXCEPT( "useCachedProxy() failed!" );
		}
	}

	server->write_line(pending_command,pending_reqid,pending_args);
	Gahp_Args return_line;
	server->read_argv(return_line);
	if ( return_line.argc == 0 || return_line.argv[0][0] != 'S' ) {
		EXCEPT("Bad %s Request: %s",pending_command, return_line.argc?return_line.argv[0]:"Empty response");
	}

	pending_submitted_to_gahp = true;
	server->num_pending_requests++;

	if (pending_timeout) {
		pending_timeout_tid = daemonCore->Register_Timer(pending_timeout + 1,
			(TimerHandlercpp)&GahpClient::reset_user_timer_alarm,
			"GahpClient::reset_user_timer_alarm",this);
		pending_timeout += time(NULL);
	}
}
