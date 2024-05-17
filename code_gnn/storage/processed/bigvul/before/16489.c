condor_net_remap_config( bool force_param )
{
    char *str = NULL;
	if( ! force_param && getenv("NET_REMAP_ENABLE") ) {
			 
		return;
	}
		
		 
	insert( "BIND_ALL_INTERFACES", "TRUE", ConfigTab, TABLESIZE );
	extra_info->AddInternalParam("BIND_ALL_INTERFACES");

    SetEnv( "NET_REMAP_ENABLE", "true");
    str = param("NET_REMAP_SERVICE");
    if (str) {
        if (!strcasecmp(str, "GCB")) {
            SetEnv( "GCB_ENABLE", "true" );
            free(str);
            str = NULL;
            if( (str = param("NET_REMAP_INAGENT")) ) {
				const char *next_broker;
				StringList all_brokers( str );
				StringList working_brokers;

				all_brokers.rewind();
				while ( (next_broker = all_brokers.next()) ) {
					int rc = 0;

#if HAVE_EXT_GCB
					int num_slots = 0;	 
					rc = GCB_broker_query( next_broker,
										   GCB_DATA_QUERY_FREE_SOCKS,
										   &num_slots );
#endif
					if ( rc == 0 ) {
						working_brokers.append( next_broker );
					}
				}

				if ( working_brokers.number() > 0 ) {
					int rand_entry = (get_random_int() % working_brokers.number()) + 1;
					int i = 0;
					working_brokers.rewind();
					while ( (i < rand_entry) &&
							(next_broker=working_brokers.next()) ) {
						i++;
					}

					dprintf( D_FULLDEBUG,"Using GCB broker %s\n",next_broker );
					SetEnv( "GCB_INAGENT", next_broker );
				} else {
					dprintf( D_ALWAYS,"No usable GCB brokers were found. "
							 "Setting GCB_INAGENT=%s\n",
							 CONDOR_GCB_INVALID_BROKER );
					SetEnv( "GCB_INAGENT", CONDOR_GCB_INVALID_BROKER );
				}
				free( str );
                str = NULL;
            }
            if( (str = param("NET_REMAP_ROUTE")) ) {
                SetEnv( "GCB_ROUTE", str );
				free( str );
                str = NULL;
            }
        } else if (!strcasecmp(str, "DPF")) {
            SetEnv( "DPF_ENABLE", "true" );
            free(str);
            str = NULL;
            if( (str = param("NET_REMAP_INAGENT")) ) {
                SetEnv( "DPF_INAGENT", str );
				free(str);
				str = NULL;
            }
            if( (str = param("NET_REMAP_ROUTE")) ) {
                SetEnv( "DPF_ROUTE", str );
				free(str);
				str = NULL;
            }
        }
    }
}
