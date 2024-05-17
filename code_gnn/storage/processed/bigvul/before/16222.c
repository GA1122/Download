GahpServer::poll()
{
	Gahp_Args* result = NULL;
	int num_results = 0;
	int i, result_reqid;
	GahpClient* entry;
	ExtArray<Gahp_Args*> result_lines;

	m_in_results = true;

	poll_pending = false;

	write_line("RESULTS");

	result = new Gahp_Args;
	ASSERT(result);
	read_argv(result);
	if ( result->argc < 2 || result->argv[0][0] != 'S' ) {
		dprintf(D_ALWAYS,"GAHP command 'RESULTS' failed\n");
		delete result;
		m_in_results = false;
		return;
	}
	num_results = atoi(result->argv[1]);

	for (i=0; i < num_results; i++) {
		if ( !result ) {
			result = new Gahp_Args;
			ASSERT(result);
		}
		read_argv(result);
		result_lines[i] = result;
		result = NULL;
	}

	m_in_results = false;
	if ( buffered_peek() > 0 ) {
		skip_next_r = true;
		poll_real_soon();
	}


	for (i=0; i < num_results; i++) {
		if ( result ) delete result;
		result = result_lines[i];

		result_reqid = 0;
		if ( result->argc > 0 ) {
			result_reqid = atoi(result->argv[0]);
		}
		if ( result_reqid == 0 ) {
			dprintf(D_ALWAYS,"GAHP - Bad RESULTS line\n");
			continue;
		}

		if ( result_reqid == globus_gt2_gram_callback_reqid ) {
			if ( result->argc == 4 ) {
				(*globus_gt2_gram_callback_func)( globus_gt2_gram_user_callback_arg, result->argv[1], 
								atoi(result->argv[2]), atoi(result->argv[3]) );
			} else {
				dprintf(D_FULLDEBUG,
					"GAHP - Bad client_callback results line\n");
			}
			continue;
		}

		if ( result_reqid == globus_gt4_gram_callback_reqid ) {
			if ( result->argc == 5 ) {
				(*globus_gt4_gram_callback_func)( globus_gt4_gram_user_callback_arg, result->argv[1], 
								result->argv[2], 
								strcmp(result->argv[3],NULLSTRING) ? result->argv[3] : NULL,
								strcmp(result->argv[4],NULLSTRING) ? atoi(result->argv[4]) : GT4_NO_EXIT_CODE );
			} else {
				dprintf(D_FULLDEBUG,
					"GAHP - Bad client_callback results line\n");
			}
			continue;
		}

		if ( result_reqid == unicore_gahp_callback_reqid ) {
			if ( result->argc == 2 ) {
				(*unicore_gahp_callback_func)( 
									strcmp(result->argv[1],NULLSTRING) ?
									result->argv[1] : NULL );
			} else {
				dprintf(D_FULLDEBUG,
					"GAHP - Bad unicore callback results line\n");
			}
			continue;
		}

		entry = NULL;
		requestTable->lookup(result_reqid,entry);
		if ( entry ) {
			entry->pending_result = result;
			result = NULL;
			entry->pending_reqid = 0;
			entry->reset_user_timer(-1);				
			num_pending_requests--;
			ASSERT(entry->pending_submitted_to_gahp);
			entry->pending_submitted_to_gahp = false;
		}
		requestTable->remove(result_reqid);

	}	 

	if ( result ) delete result;


	ASSERT(num_pending_requests >= 0);
	int waiting_reqid = -1;
	while ( num_pending_requests < max_pending_requests )
	{
		if ( waitingHighPrio.size() > 0 ) {
			waiting_reqid = waitingHighPrio.front();
			waitingHighPrio.pop();
		} else if ( waitingMediumPrio.size() > 0 ) {
			waiting_reqid = waitingMediumPrio.front();
			waitingMediumPrio.pop();
		} else if ( waitingLowPrio.size() > 0 ) {
			waiting_reqid = waitingLowPrio.front();
			waitingLowPrio.pop();
		} else {
			break;
		}
		entry = NULL;
		requestTable->lookup(waiting_reqid,entry);
		if ( entry ) {
			ASSERT(entry->pending_reqid == waiting_reqid);
			entry->now_pending(NULL,NULL);
		} else {
			requestTable->remove(result_reqid);
		}
	}
}
