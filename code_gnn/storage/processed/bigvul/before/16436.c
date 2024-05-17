CStarter::startSSHD( int  , Stream* s )
{
	MyString error_msg;
	Sock *sock = (Sock*)s;
	char const *fqu = sock->getFullyQualifiedUser();
	MyString job_owner;
	getJobOwnerFQUOrDummy(job_owner);
	if( !fqu || job_owner != fqu ) {
		dprintf(D_ALWAYS,"Unauthorized attempt to start sshd by '%s'\n",
				fqu ? fqu : "");
		return FALSE;
	}

	ClassAd input;
	s->decode();
	if( !input.initFromStream(*s) || !s->end_of_message() ) {
		dprintf(D_ALWAYS,"Failed to read request in START_SSHD.\n");
		return FALSE;
	}

#if !defined(HAVE_SSH_TO_JOB)
	return SSHDFailed(s,"This version of condor_starter does not support ssh access.");
#else

	ClassAd *jobad = NULL;
	ClassAd *machinead = NULL;
	if( jic ) {
		jobad = jic->jobClassAd();
		machinead = jic->machClassAd();
	}

	bool enabled = param_boolean("ENABLE_SSH_TO_JOB",true,true,machinead,jobad);
	if( !enabled ) {
		return SSHDFailed(s,"Rejecting request, because ENABLE_SSH_TO_JOB=false");
	}

	if( !jic || !jobad ) {
		return SSHDRetry(s,"Rejecting request, because job not yet initialized.");
	}
	if( !m_job_environment_is_ready ) {
		return SSHDRetry(s,"Rejecting request, because the job execution environment is not yet ready.");
	}
	if( m_all_jobs_done ) {
		return SSHDFailed(s,"Rejecting request, because the job is finished.");
	}
	if( suspended ) {
		return SSHDRetry(s,"This slot is currently suspended.");
	}

	MyString preferred_shells;
	input.LookupString(ATTR_SHELL,preferred_shells);

	MyString slot_name;
	input.LookupString(ATTR_NAME,slot_name);

	if( !jic->userPrivInitialized() ) {
		return SSHDRetry(s,"Rejecting request, because job execution account not yet established.");
	}

	MyString libexec;
	if( !param(libexec,"LIBEXEC") ) {
		return SSHDFailed(s,"LIBEXEC not defined, so cannot find condor_ssh_to_job_sshd_setup");
	}
	MyString ssh_to_job_sshd_setup;
	MyString ssh_to_job_shell_setup;
	ssh_to_job_sshd_setup.sprintf(
		"%s%ccondor_ssh_to_job_sshd_setup",libexec.Value(),DIR_DELIM_CHAR);
	ssh_to_job_shell_setup.sprintf(
		"%s%ccondor_ssh_to_job_shell_setup",libexec.Value(),DIR_DELIM_CHAR);

	if( access(ssh_to_job_sshd_setup.Value(),X_OK)!=0 ) {
		return SSHDFailed(s,"Cannot execute %s: %s",
						  ssh_to_job_sshd_setup.Value(),strerror(errno));
	}
	if( access(ssh_to_job_shell_setup.Value(),X_OK)!=0 ) {
		return SSHDFailed(s,"Cannot execute %s: %s",
						  ssh_to_job_shell_setup.Value(),strerror(errno));
	}

	MyString sshd_config_template;
	if( !param(sshd_config_template,"SSH_TO_JOB_SSHD_CONFIG_TEMPLATE") ) {
		if( param(sshd_config_template,"LIB") ) {
			sshd_config_template.sprintf_cat("%ccondor_ssh_to_job_sshd_config_template",DIR_DELIM_CHAR);
		}
		else {
			return SSHDFailed(s,"SSH_TO_JOB_SSHD_CONFIG_TEMPLATE and LIB are not defined.  At least one of them is required.");
		}
	}
	if( access(sshd_config_template.Value(),F_OK)!=0 ) {
		return SSHDFailed(s,"%s does not exist!",sshd_config_template.Value());
	}

	MyString ssh_keygen;
	MyString ssh_keygen_args;
	ArgList ssh_keygen_arglist;
	param(ssh_keygen,"SSH_TO_JOB_SSH_KEYGEN","/usr/bin/ssh-keygen");
	param(ssh_keygen_args,"SSH_TO_JOB_SSH_KEYGEN_ARGS","\"-N '' -C '' -q -f %f -t rsa\"");
	ssh_keygen_arglist.AppendArg(ssh_keygen.Value());
	if( !ssh_keygen_arglist.AppendArgsV2Quoted(ssh_keygen_args.Value(),&error_msg) ) {
		return SSHDFailed(s,
						  "SSH_TO_JOB_SSH_KEYGEN_ARGS is misconfigured: %s",
						  error_msg.Value());
	}

	MyString client_keygen_args;
	input.LookupString(ATTR_SSH_KEYGEN_ARGS,client_keygen_args);
	if( !ssh_keygen_arglist.AppendArgsV2Raw(client_keygen_args.Value(),&error_msg) ) {
		return SSHDFailed(s,
						  "Failed to produce ssh-keygen arg list: %s",
						  error_msg.Value());
	}

	MyString ssh_keygen_cmd;
	if(!ssh_keygen_arglist.GetArgsStringSystem(&ssh_keygen_cmd,0,&error_msg)) {
		return SSHDFailed(s,
						  "Failed to produce ssh-keygen command string: %s",
						  error_msg.Value());
	}

	int setup_pipe_fds[2];
	setup_pipe_fds[0] = -1;
	setup_pipe_fds[1] = -1;
	if( !daemonCore->Create_Pipe(setup_pipe_fds) ) {
		return SSHDFailed(
			s,"Failed to create pipe for condor_ssh_to_job_sshd_setup.");
	}
	int setup_std_fds[3];
	setup_std_fds[0] = 0;
	setup_std_fds[1] = setup_pipe_fds[1];  
	setup_std_fds[2] = setup_pipe_fds[1];

	Env setup_env;
	if( !GetJobEnv( jobad, &setup_env, &error_msg ) ) {
		return SSHDFailed(
			s,"Failed to get job environment: %s",error_msg.Value());
	}

	if( !slot_name.IsEmpty() ) {
		setup_env.SetEnv("_CONDOR_SLOT_NAME",slot_name.Value());
	}

    int setup_opt_mask = 0;
    if (!param_boolean("JOB_INHERITS_STARTER_ENVIRONMENT",false)) {
        setup_opt_mask =  DCJOBOPT_NO_ENV_INHERIT;
    }

	if( !preferred_shells.IsEmpty() ) {
		dprintf(D_FULLDEBUG,
				"Checking preferred shells: %s\n",preferred_shells.Value());
		StringList shells(preferred_shells.Value(),",");
		shells.rewind();
		char *shell;
		while( (shell=shells.next()) ) {
			if( access(shell,X_OK)==0 ) {
				dprintf(D_FULLDEBUG,"Will use shell %s\n",shell);
				setup_env.SetEnv("_CONDOR_SHELL",shell);
				break;
			}
		}
	}

	ArgList setup_args;
	setup_args.AppendArg(ssh_to_job_sshd_setup.Value());
	setup_args.AppendArg(GetWorkingDir());
	setup_args.AppendArg(ssh_to_job_shell_setup.Value());
	setup_args.AppendArg(sshd_config_template.Value());
	setup_args.AppendArg(ssh_keygen_cmd.Value());

	int setup_reaper = 1;
	int setup_pid;
	if( privSepHelper() ) {
		setup_pid = privSepHelper()->create_process(
			ssh_to_job_sshd_setup.Value(),
			setup_args,
			setup_env,
			GetWorkingDir(),
			setup_std_fds,
			NULL,
			0,
			NULL,
			setup_reaper,
			setup_opt_mask,
			NULL);
	}
	else {
		setup_pid = daemonCore->Create_Process(
			ssh_to_job_sshd_setup.Value(),
			setup_args,
			PRIV_USER_FINAL,
			setup_reaper,
			FALSE,
			&setup_env,
			GetWorkingDir(),
			NULL,
			NULL,
			setup_std_fds,
			NULL,
			0,
			NULL,
			setup_opt_mask);
	}

	daemonCore->Close_Pipe(setup_pipe_fds[1]);  

	char *setup_output = NULL;
	int setup_output_len = 0;
	char pipe_buf[1024];
	while( true ) {
		int n = daemonCore->Read_Pipe(setup_pipe_fds[0],pipe_buf,1024);
		if( n <= 0 ) {
			break;
		}
		char *old_setup_output = setup_output;
		setup_output = (char *)realloc(setup_output,setup_output_len+n+1);
		if( !setup_output ) {
			free( old_setup_output );
			daemonCore->Close_Pipe(setup_pipe_fds[0]);  
			return SSHDFailed(s,"Out of memory");
		}
		memcpy(setup_output+setup_output_len,pipe_buf,n);
		setup_output_len += n;
		setup_output[setup_output_len] = '\0';
	}

	daemonCore->Close_Pipe(setup_pipe_fds[0]);  

	if( find_str_in_buffer(setup_output,setup_output_len,"condor_ssh_to_job_sshd_setup SUCCESS") < 0 ) {
		error_msg.sprintf("condor_ssh_to_job_sshd_setup failed: %s",
						  setup_output);
		free( setup_output );
		return SSHDFailed(s,"%s",error_msg.Value());
	}


	bool rc = true;
	MyString session_dir;
	if( rc ) {
		rc = extract_delimited_data(
			setup_output,
			setup_output_len,
			"condor_ssh_to_job_sshd_setup SSHD DIR BEGIN\n",
			"\ncondor_ssh_to_job_sshd_setup SSHD DIR END\n",
			session_dir,
			&error_msg);
	}

	MyString sshd_user;
	if( rc ) {
		rc = extract_delimited_data(
			setup_output,
			setup_output_len,
			"condor_ssh_to_job_sshd_setup SSHD USER BEGIN\n",
			"\ncondor_ssh_to_job_sshd_setup SSHD USER END\n",
			sshd_user,
			&error_msg);
	}

	MyString public_host_key;
	if( rc ) {
		rc = extract_delimited_data_as_base64(
			setup_output,
			setup_output_len,
			"condor_ssh_to_job_sshd_setup PUBLIC SERVER KEY BEGIN\n",
			"condor_ssh_to_job_sshd_setup PUBLIC SERVER KEY END\n",
			public_host_key,
			&error_msg);
	}

	MyString private_client_key;
	if( rc ) {
		rc = extract_delimited_data_as_base64(
			setup_output,
			setup_output_len,
			"condor_ssh_to_job_sshd_setup AUTHORIZED CLIENT KEY BEGIN\n",
			"condor_ssh_to_job_sshd_setup AUTHORIZED CLIENT KEY END\n",
			private_client_key,
			&error_msg);
	}

	free( setup_output );

	if( !rc ) {
		MyString msg;
		return SSHDFailed(s,
			"Failed to parse output of condor_ssh_to_job_sshd_setup: %s",
			error_msg.Value());
	}

	dprintf(D_FULLDEBUG,"StartSSHD: session_dir='%s'\n",session_dir.Value());

	MyString sshd_config_file;
	sshd_config_file.sprintf("%s%csshd_config",session_dir.Value(),DIR_DELIM_CHAR);



	MyString sshd;
	param(sshd,"SSH_TO_JOB_SSHD","/usr/sbin/sshd");
	if( access(sshd.Value(),X_OK)!=0 ) {
		return SSHDFailed(s,"Failed, because sshd not correctly configured (SSH_TO_JOB_SSHD=%s): %s.",sshd.Value(),strerror(errno));
	}

	ArgList sshd_arglist;
	MyString sshd_arg_string;
	param(sshd_arg_string,"SSH_TO_JOB_SSHD_ARGS","\"-i -e -f %f\"");
	if( !sshd_arglist.AppendArgsV2Quoted(sshd_arg_string.Value(),&error_msg) )
	{
		return SSHDFailed(s,"Invalid SSH_TO_JOB_SSHD_ARGS (%s): %s",
						  sshd_arg_string.Value(),error_msg.Value());
	}

	char **argarray = sshd_arglist.GetStringArray();
	sshd_arglist.Clear();
	for(int i=0; argarray[i]; i++) {
		char const *ptr;
		MyString new_arg;
		for(ptr=argarray[i]; *ptr; ptr++) {
			if( *ptr == '%' ) {
				ptr += 1;
				if( *ptr == '%' ) {
					new_arg += '%';
				}
				else if( *ptr == 'f' ) {
					new_arg += sshd_config_file.Value();
				}
				else {
					return SSHDFailed(s,
							"Unexpected %%%c in SSH_TO_JOB_SSHD_ARGS: %s\n",
							*ptr ? *ptr : ' ', sshd_arg_string.Value());
				}
			}
			else {
				new_arg += *ptr;
			}
		}
		sshd_arglist.AppendArg(new_arg.Value());
	}
	deleteStringArray(argarray);
	argarray = NULL;


	ClassAd sshd_ad;
	sshd_ad.CopyAttribute(ATTR_REMOTE_USER,jobad);
	sshd_ad.CopyAttribute(ATTR_JOB_RUNAS_OWNER,jobad);
	sshd_ad.Assign(ATTR_JOB_CMD,sshd.Value());
	CondorVersionInfo ver_info;
	if( !sshd_arglist.InsertArgsIntoClassAd(&sshd_ad,&ver_info,&error_msg) ) {
		return SSHDFailed(s,
			"Failed to insert args into sshd job description: %s",
			error_msg.Value());
	}
	if( !setup_env.InsertEnvIntoClassAd(&sshd_ad,&error_msg,NULL,&ver_info) ) {
		return SSHDFailed(s,
			"Failed to insert environment into sshd job description: %s",
			error_msg.Value());
	}



	ClassAd response;
	response.Assign(ATTR_RESULT,true);
	response.Assign(ATTR_REMOTE_USER,sshd_user);
	response.Assign(ATTR_SSH_PUBLIC_SERVER_KEY,public_host_key.Value());
	response.Assign(ATTR_SSH_PRIVATE_CLIENT_KEY,private_client_key.Value());

	s->encode();
	if( !response.put(*s) || !s->end_of_message() ) {
		dprintf(D_ALWAYS,"Failed to send response to START_SSHD.\n");
		return FALSE;
	}



	MyString sshd_log_fname;
	sshd_log_fname.sprintf(
		"%s%c%s",session_dir.Value(),DIR_DELIM_CHAR,"sshd.log");


	int std[3];
	char const *std_fname[3];
	std[0] = sock->get_file_desc();
	std_fname[0] = "stdin";
	std[1] = sock->get_file_desc();
	std_fname[1] = "stdout";
	std[2] = -1;
	std_fname[2] = sshd_log_fname.Value();


	SSHDProc *proc = new SSHDProc(&sshd_ad);
	if( !proc ) {
		dprintf(D_ALWAYS,"Failed to create SSHDProc.\n");
		return FALSE;
	}
	if( !proc->StartJob(std,std_fname) ) {
		dprintf(D_ALWAYS,"Failed to start sshd.\n");
		return FALSE;
	}
	m_job_list.Append(proc);
	if( this->suspended ) {
		proc->Suspend();
	}

	return TRUE;
#endif
}
