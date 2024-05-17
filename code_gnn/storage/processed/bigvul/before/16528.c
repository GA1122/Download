reinsert_specials( char* host )
{
	static unsigned int reinsert_pid = 0;
	static unsigned int reinsert_ppid = 0;
	static bool warned_no_user = false;
	char buf[40];

	if( tilde ) {
		insert( "TILDE", tilde, ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("TILDE");
	}
	if( host ) {
		insert( "HOSTNAME", host, ConfigTab, TABLESIZE );
	} else {
		insert( "HOSTNAME", get_local_hostname().Value(), ConfigTab, 
				TABLESIZE );
	}
	insert( "FULL_HOSTNAME", get_local_fqdn().Value(), ConfigTab, TABLESIZE );
	insert( "SUBSYSTEM", get_mySubSystem()->getName(), ConfigTab, TABLESIZE );
	extra_info->AddInternalParam("HOSTNAME");
	extra_info->AddInternalParam("FULL_HOSTNAME");
	extra_info->AddInternalParam("SUBSYSTEM");

	char *myusernm = my_username();
	if( myusernm ) {
		insert( "USERNAME", myusernm, ConfigTab, TABLESIZE );
		free(myusernm);
		myusernm = NULL;
		extra_info->AddInternalParam("USERNAME");
	} else {
		if( ! warned_no_user ) {
			dprintf( D_ALWAYS, "ERROR: can't find username of current user! "
					 "BEWARE: $(USERNAME) will be undefined\n" );
			warned_no_user = true;
		}
	}

	{
		uid_t myruid;
		gid_t myrgid;
#ifdef WIN32
		myruid = 666;
		myrgid = 666;
#else
		myruid = getuid();
		myrgid = getgid();
#endif
		snprintf(buf,40,"%u",myruid);
		insert( "REAL_UID", buf, ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("REAL_UID");
		snprintf(buf,40,"%u",myrgid);
		insert( "REAL_GID", buf, ConfigTab, TABLESIZE );
		extra_info->AddInternalParam("REAL_GID");
	}
		
	if (!reinsert_pid) {
#ifdef WIN32
		reinsert_pid = ::GetCurrentProcessId();
#else
		reinsert_pid = getpid();
#endif
	}
	snprintf(buf,40,"%u",reinsert_pid);
	insert( "PID", buf, ConfigTab, TABLESIZE );
	extra_info->AddInternalParam("PID");
	if ( !reinsert_ppid ) {
#ifdef WIN32
		CSysinfo system_hackery;
		reinsert_ppid = system_hackery.GetParentPID(reinsert_pid);
#else
		reinsert_ppid = getppid();
#endif
	}
	snprintf(buf,40,"%u",reinsert_ppid);
	insert( "PPID", buf, ConfigTab, TABLESIZE );
	insert( "IP_ADDRESS", my_ip_string(), ConfigTab, TABLESIZE );
	extra_info->AddInternalParam("PPID");
	extra_info->AddInternalParam("IP_ADDRESS");
}
