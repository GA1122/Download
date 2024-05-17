FileTransfer::SimpleInit(ClassAd *Ad, bool want_check_perms, bool is_server, 
						 ReliSock *sock_to_use, priv_state priv,
						 bool use_file_catalog, bool is_spool) 
{
	char buf[ATTRLIST_MAX_EXPRESSION];
	char *dynamic_buf = NULL;

	jobAd = *Ad;	 

	if( did_init ) {
		return 1;
	}

	user_supplied_key = is_server ? FALSE : TRUE;

	dprintf(D_FULLDEBUG,"entering FileTransfer::SimpleInit\n");

	 
	m_use_file_catalog = use_file_catalog;

	desired_priv_state = priv;
    if ( priv == PRIV_UNKNOWN ) {
        want_priv_change = false;
    } else {
        want_priv_change = true;
    }

	simple_sock = sock_to_use;

	if (Ad->LookupString(ATTR_JOB_IWD, buf) != 1) {
		dprintf(D_FULLDEBUG, 
			"FileTransfer::SimpleInit: Job Ad did not have an iwd!\n");
		return 0;
	}
	Iwd = strdup(buf);

	if ( want_check_perms ) {
		if (Ad->LookupString(ATTR_OWNER, buf) != 1) {
			dprintf(D_FULLDEBUG, 
				"FileTransfer::SimpleInit: Job Ad did not have an owner!\n");
			return 0;		
		}
#ifdef WIN32
		char ntdomain[80];
		char *p_ntdomain = ntdomain;
		if (Ad->LookupString(ATTR_NT_DOMAIN, ntdomain) != 1) {
			p_ntdomain = NULL;
		}
		perm_obj = new perm();
		if ( !perm_obj->init(buf,p_ntdomain) ) {
			delete perm_obj;
			perm_obj = NULL;
			return 0;
		} 
#endif
	}

	dynamic_buf = NULL;
	if (Ad->LookupString(ATTR_TRANSFER_INPUT_FILES, &dynamic_buf) == 1) {
		InputFiles = new StringList(dynamic_buf,",");
		free(dynamic_buf);
		dynamic_buf = NULL;
	} else {
		InputFiles = new StringList(NULL,",");
	}
	if (Ad->LookupString(ATTR_JOB_INPUT, buf) == 1) {
		if ( ! nullFile(buf) ) {			
			if ( !InputFiles->file_contains(buf) )
				InputFiles->append(buf);			
		}
	}

	if (IsClient() && simple_init && is_spool) {
		InputFiles->rewind();
		const char *x;
		while ((x = InputFiles->next())) {
			if (IsUrl(x)) {
				InputFiles->deleteCurrent();
			}
		}
		char *list = InputFiles->print_to_string();
		dprintf(D_FULLDEBUG, "Input files: %s\n", list ? list : "" );
		free(list);
	}
	
	if ( Ad->LookupString(ATTR_ULOG_FILE, buf) == 1 ) {
		UserLogFile = strdup(condor_basename(buf));
	}
	if ( Ad->LookupString(ATTR_X509_USER_PROXY, buf) == 1 ) {
		X509UserProxy = strdup(buf);
		if ( !nullFile(buf) ) {			
			if ( !InputFiles->file_contains(buf) )
				InputFiles->append(buf);			
		}
	}
	if ( Ad->LookupString(ATTR_OUTPUT_DESTINATION, buf) == 1 ) {
		OutputDestination = strdup(buf);
		dprintf(D_FULLDEBUG, "FILETRANSFER: using OutputDestination %s\n", buf);
	}

	char* Spool = NULL;
	if ( IsServer() ) {
		Spool = param("SPOOL");
	}

	int Cluster = 0;
	int Proc = 0;
	Ad->LookupInteger(ATTR_CLUSTER_ID, Cluster);
	Ad->LookupInteger(ATTR_PROC_ID, Proc);
	m_jobid.sprintf("%d.%d",Cluster,Proc);
	if ( IsServer() && Spool ) {

		SpoolSpace = gen_ckpt_name(Spool,Cluster,Proc,0);
		TmpSpoolSpace = (char*)malloc( strlen(SpoolSpace) + 10 );
		sprintf(TmpSpoolSpace,"%s.tmp",SpoolSpace);
	}

	if ( ((IsServer() && !simple_init) || (IsClient() && simple_init)) && 
		 (Ad->LookupString(ATTR_JOB_CMD, buf) == 1) ) 
	{

		if ( IsServer() && Spool ) {
			ExecFile = gen_ckpt_name(Spool,Cluster,ICKPT,0);
			if ( access(ExecFile,F_OK | X_OK) < 0 ) {
				free(ExecFile); ExecFile = NULL;
			}
		}

		if ( !ExecFile ) {
#ifdef WIN32
			if ( perm_obj && (perm_obj->read_access(buf) != 1) ) {
				dprintf(D_ALWAYS,
					"FileTrans: permission denied reading %s\n",buf);
				return 0;
			}
#endif
			ExecFile = strdup(buf);
		}


		int xferExec;
		if(!Ad->LookupBool(ATTR_TRANSFER_EXECUTABLE,xferExec)) {
			xferExec=1;
		}

		if ( xferExec && !InputFiles->file_contains(ExecFile) ) {
			InputFiles->append(ExecFile);	
		}	
	}

	dynamic_buf = NULL;
	if (Ad->LookupString(ATTR_SPOOLED_OUTPUT_FILES, &dynamic_buf) == 1 ||
		Ad->LookupString(ATTR_TRANSFER_OUTPUT_FILES, &dynamic_buf) == 1)
	{
		OutputFiles = new StringList(dynamic_buf,",");
		free(dynamic_buf);
		dynamic_buf = NULL;
	} else {
		upload_changed_files = true;
	}
	int streaming = 0;
	JobStdoutFile = "";
	if(Ad->LookupString(ATTR_JOB_OUTPUT, buf) == 1 ) {
		JobStdoutFile = buf;
		Ad->LookupBool( ATTR_STREAM_OUTPUT, streaming );
		if( ! streaming && ! upload_changed_files && ! nullFile(buf) ) {
			if( OutputFiles ) {
				if( !OutputFiles->file_contains(buf) ) {
					OutputFiles->append( buf );
				}
			} else {
				OutputFiles = new StringList(buf,",");
			}
		}
	}
	streaming = 0;
	JobStderrFile = "";
	if( Ad->LookupString(ATTR_JOB_ERROR, buf) == 1 ) {
		JobStderrFile = buf;
		Ad->LookupBool( ATTR_STREAM_ERROR, streaming );
		if( ! streaming && ! upload_changed_files && ! nullFile(buf) ) {
			if( OutputFiles ) {
				if( !OutputFiles->file_contains(buf) ) {
					OutputFiles->append( buf );
				}
			} else {
				OutputFiles = new StringList(buf,",");
			}
		}
	}

	MyString ulog;
	if( jobAd.LookupString(ATTR_ULOG_FILE,ulog) ) {
		if( outputFileIsSpooled(ulog.Value()) ) {
			if( OutputFiles ) {
				if( !OutputFiles->file_contains(ulog.Value()) ) {
					OutputFiles->append(ulog.Value());
				}
			} else {
				OutputFiles = new StringList(buf,",");
			}
		}
	}

	if (Ad->LookupString(ATTR_ENCRYPT_INPUT_FILES, buf) == 1) {
		EncryptInputFiles = new StringList(buf,",");
	} else {
		EncryptInputFiles = new StringList(NULL,",");
	}

	if (Ad->LookupString(ATTR_ENCRYPT_OUTPUT_FILES, buf) == 1) {
		EncryptOutputFiles = new StringList(buf,",");
	} else {
		EncryptOutputFiles = new StringList(NULL,",");
	}

	if (Ad->LookupString(ATTR_DONT_ENCRYPT_INPUT_FILES, buf) == 1) {
		DontEncryptInputFiles = new StringList(buf,",");
	} else {
		DontEncryptInputFiles = new StringList(NULL,",");
	}

	if (Ad->LookupString(ATTR_DONT_ENCRYPT_OUTPUT_FILES, buf) == 1) {
		DontEncryptOutputFiles = new StringList(buf,",");
	} else {
		DontEncryptOutputFiles = new StringList(NULL,",");
	}



	bool spooling_output = false;
	{
		if (Iwd && Spool) {
			if(!strncmp(Iwd,Spool,strlen(Spool))) {
				spooling_output = true;
			}
		}
	}

	if(IsServer() && !spooling_output) {
		if(!InitDownloadFilenameRemaps(Ad)) return 0;
	}

	CondorError e;
	I_support_filetransfer_plugins = false;
	plugin_table = NULL;
	InitializePlugins(e);

	int spool_completion_time = 0;
	Ad->LookupInteger(ATTR_STAGE_IN_FINISH,spool_completion_time);
	last_download_time = spool_completion_time;
	if(IsServer()) {
		BuildFileCatalog(last_download_time);
	} else {
		BuildFileCatalog();
	}

	if ( Spool ) {
		free(Spool);
	}

	did_init = true;
	return 1;
}
