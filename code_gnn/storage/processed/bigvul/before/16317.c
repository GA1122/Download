BaseShadow::baseInit( ClassAd *job_ad, const char* schedd_addr, const char *xfer_queue_contact_info )
{
	int pending = FALSE;

	if( ! job_ad ) {
		EXCEPT("baseInit() called with NULL job_ad!");
	}
	jobAd = job_ad;

	if (sendUpdatesToSchedd && ! is_valid_sinful(schedd_addr)) {
		EXCEPT("schedd_addr not specified with valid address");
	}
	scheddAddr = sendUpdatesToSchedd ? strdup( schedd_addr ) : strdup("noschedd");

	m_xfer_queue_contact_info = xfer_queue_contact_info;

	if ( !jobAd->LookupString(ATTR_OWNER, owner)) {
		EXCEPT("Job ad doesn't contain an %s attribute.", ATTR_OWNER);
	}

	if( !jobAd->LookupInteger(ATTR_CLUSTER_ID, cluster)) {
		EXCEPT("Job ad doesn't contain an %s attribute.", ATTR_CLUSTER_ID);
	}

	if( !jobAd->LookupInteger(ATTR_PROC_ID, proc)) {
		EXCEPT("Job ad doesn't contain an %s attribute.", ATTR_PROC_ID);
	}


	if( ! jobAd->LookupString(ATTR_GLOBAL_JOB_ID, &gjid) ) {
		gjid = NULL;
	}

	jobAd->LookupString(ATTR_NT_DOMAIN, domain);
	if ( !jobAd->LookupString(ATTR_JOB_IWD, iwd)) {
		EXCEPT("Job ad doesn't contain an %s attribute.", ATTR_JOB_IWD);
	}

	if( !jobAd->LookupFloat(ATTR_BYTES_SENT, prev_run_bytes_sent) ) {
		prev_run_bytes_sent = 0;
	}
	if( !jobAd->LookupFloat(ATTR_BYTES_RECVD, prev_run_bytes_recvd) ) {
		prev_run_bytes_recvd = 0;
	}

	MyString tmp_name = iwd;
	tmp_name += DIR_DELIM_CHAR;
	tmp_name += "core.";
	tmp_name += cluster;
	tmp_name += '.';
	tmp_name += proc;
	core_file_name = strdup( tmp_name.Value() );

	MyString tmp_addr = ATTR_MY_ADDRESS;
	tmp_addr += "=\"";
	tmp_addr += daemonCore->InfoCommandSinfulString();
	tmp_addr += '"';
    if ( !jobAd->Insert( tmp_addr.Value() )) {
        EXCEPT( "Failed to insert %s!", ATTR_MY_ADDRESS );
    }

	DebugId = display_dprintf_header;
	
	config();

	checkSwap();

	if ( !init_user_ids(owner.Value(), domain.Value())) {
		dprintf(D_ALWAYS, "init_user_ids() failed as user %s\n",owner.Value() );
		
#if ! defined(WIN32)
		if ( param_boolean( "SHADOW_RUN_UNKNOWN_USER_JOBS", false ) )
		{
			dprintf(D_ALWAYS, "trying init_user_ids() as user nobody\n" );
			
			owner="nobody";
			domain=NULL;
			if (!init_user_ids(owner.Value(), domain.Value()))
			{
				dprintf(D_ALWAYS, "init_user_ids() failed!\n");
			}
			else
			{
				jobAd->Assign( ATTR_JOB_RUNAS_OWNER, "FALSE" );
				m_RunAsNobody=true;
				dprintf(D_ALWAYS, "init_user_ids() now running as user nobody\n");
			}
		}
#endif

	}
	set_user_priv();
	daemonCore->Register_Priv_State( PRIV_USER );

	dumpClassad( "BaseShadow::baseInit()", this->jobAd, D_JOB );

	shadow_user_policy.init( jobAd, this );


	if (sendUpdatesToSchedd) {
		job_updater = new QmgrJobUpdater( jobAd, scheddAddr, CondorVersion() );
	} else {
		job_updater = new NullQmgrJobUpdater( jobAd, scheddAddr, CondorVersion() );
	}

	initUserLog();

	if ( cdToIwd() == -1 ) {
		EXCEPT("Could not cd to initial working directory");
	}

	if( jobAd->LookupInteger(ATTR_TERMINATION_PENDING, pending)) {
		if (pending == TRUE) {
			this->terminateJob(US_TERMINATE_PENDING);
		}
	}

	int wantClaiming = 0;
	jobAd->LookupBool(ATTR_CLAIM_STARTD, wantClaiming);
	if (wantClaiming) {
		MyString startdSinful;
		MyString claimid;

		jobAd->LookupString(ATTR_STARTD_IP_ADDR, startdSinful);
		jobAd->LookupString(ATTR_CLAIM_ID, claimid);

		dprintf(D_ALWAYS, "%s is true, trying to claim startd %s\n", ATTR_CLAIM_STARTD, startdSinful.Value());

		classy_counted_ptr<DCStartd> startd = new DCStartd("description", NULL, startdSinful.Value(), claimid.Value());
	
		classy_counted_ptr<DCMsgCallback> cb = 
			new DCMsgCallback((DCMsgCallback::CppFunction)&BaseShadow::startdClaimedCB,
			this, jobAd);
																 
		startd->asyncRequestOpportunisticClaim(jobAd, 
											   "description", 
											   daemonCore->InfoCommandSinfulString(), 
											   1200  , 
											   20  , 
											   100  , 
											   cb);
	}
}
