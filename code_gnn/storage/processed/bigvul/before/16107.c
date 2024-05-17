int RefreshProxyThruMyProxy(X509CredentialWrapper * proxy)
{
  const char * proxy_filename = proxy->GetStorageName();
  char * myproxy_host = NULL;
  int status;

  if (((X509Credential*)proxy->cred)->GetMyProxyServerHost() == NULL) {
    dprintf (D_ALWAYS, "Skipping %s\n", proxy->cred->GetName());
    return FALSE;
  }

  time_t now = time(NULL);

  if (proxy->get_delegation_pid != GET_DELEGATION_PID_NONE) {
    time_t time_started = proxy->get_delegation_proc_start_time;

    if (now - time_started > 500) {
      dprintf (D_FULLDEBUG, "MyProxy refresh process pid=%d still running, "
			  "sending signal %d\n",
			   proxy->get_delegation_pid, SIGKILL);
      daemonCore->Send_Signal (proxy->get_delegation_pid, SIGKILL);
    } else {
      dprintf (D_FULLDEBUG, "MyProxy refresh process pid=%d still running, "
			  "letting it finish\n",
			   proxy->get_delegation_pid);
	}
	return FALSE;
  }

  proxy->get_delegation_proc_start_time = now;

  Env myEnv;
  MyString strBuff;

  if (((X509Credential*)proxy->cred)->GetMyProxyServerDN()) {
    strBuff="MYPROXY_SERVER_DN=";
    strBuff+= ((X509Credential*)proxy->cred)->GetMyProxyServerDN();
    myEnv.SetEnv (strBuff.Value());
    dprintf (D_FULLDEBUG, "%s\n", strBuff.Value());
  }

  strBuff="X509_USER_PROXY=";
  strBuff+=proxy->GetStorageName();
  dprintf (D_FULLDEBUG, "%s\n", strBuff.Value());

  const char * myproxy_password =((X509Credential*)proxy->cred)->GetRefreshPassword();
  if (myproxy_password == NULL ) {
    dprintf (D_ALWAYS, "No MyProxy password specified for %s:%s\n",
	     proxy->cred->GetName(),
	     proxy->cred->GetOwner());
    myproxy_password = "";
  }

  status = pipe (proxy->get_delegation_password_pipe);
  if (status == -1) {
	dprintf (D_ALWAYS, "get_delegation pipe() failed: %s\n", strerror(errno) );
	proxy->get_delegation_reset();
	return FALSE;
  }
  write (proxy->get_delegation_password_pipe[1],
	 myproxy_password,
	 strlen (myproxy_password));
  write (proxy->get_delegation_password_pipe[1], "\n", 1);


  const char * username = proxy->cred->GetOrigOwner();

  myproxy_host = getHostFromAddr (((X509Credential*)proxy->cred)->GetMyProxyServerHost());
  int myproxy_port = getPortFromAddr (((X509Credential*)proxy->cred)->GetMyProxyServerHost());

  ArgList args;
  args.AppendArg("--verbose ");

  args.AppendArg("--out");
  args.AppendArg(proxy_filename);

  args.AppendArg("--pshost");
  args.AppendArg(myproxy_host);
  if ( myproxy_host != NULL ) {
	  free ( myproxy_host );
  }

  args.AppendArg("--dn_as_username");

  args.AppendArg("--proxy_lifetime");	 
  args.AppendArg(6);

  args.AppendArg("--stdin_pass");

  args.AppendArg("--username");
  args.AppendArg(username);

  if (myproxy_port) {
	  args.AppendArg("--psport");
	  args.AppendArg(myproxy_port);
  }

  if	(	((X509Credential*)proxy->cred)->GetCredentialName() && 
  			( ((X509Credential*)proxy->cred)->GetCredentialName() )[0] ) {
	  args.AppendArg("--credname");
	  args.AppendArg(((X509Credential*)proxy->cred)->GetCredentialName());
  }



  priv_state priv = set_condor_priv();
  proxy->get_delegation_err_filename = create_temp_file();
  if (proxy->get_delegation_err_filename == NULL) {
	dprintf (D_ALWAYS, "get_delegation create_temp_file() failed: %s\n",
			strerror(errno) );
	proxy->get_delegation_reset();
	return FALSE;
  }
  status = chmod (proxy->get_delegation_err_filename, 0600);
  if (status == -1) {
	dprintf (D_ALWAYS, "chmod() get_delegation_err_filename %s failed: %s\n",
			proxy->get_delegation_err_filename, strerror(errno) );
	proxy->get_delegation_reset();
	return FALSE;
  }


  proxy->get_delegation_err_fd = safe_open_wrapper_follow(proxy->get_delegation_err_filename,O_RDWR);
  if (proxy->get_delegation_err_fd == -1) {
    dprintf (D_ALWAYS, "Error opening get_delegation file %s: %s\n",
	     proxy->get_delegation_err_filename, strerror(errno) );
	proxy->get_delegation_reset();
	return FALSE;
  }
  set_priv (priv);


  int arrIO[3];
  arrIO[0]=proxy->get_delegation_password_pipe[0];  
  arrIO[1]=-1;  
  arrIO[2]=proxy->get_delegation_err_fd;  


  char * myproxy_get_delegation_pgm = param ("MYPROXY_GET_DELEGATION");
  if (!myproxy_get_delegation_pgm) {
    dprintf (D_ALWAYS, "MYPROXY_GET_DELEGATION not defined in config file\n");
    return FALSE;
  }
  MyString args_string;
  args.GetArgsStringForDisplay(&args_string);
  dprintf (D_ALWAYS, "Calling %s %s\n", myproxy_get_delegation_pgm, args_string.Value());

  int pid = daemonCore->Create_Process (
					myproxy_get_delegation_pgm,		 
					args,				 			 
					PRIV_USER_FINAL,				 
					myproxyGetDelegationReaperId,	 
					FALSE,							 
					&myEnv,							 
					NULL,							 
					NULL,							 
					NULL,							 
					arrIO);							 
  free (myproxy_get_delegation_pgm);
  myproxy_get_delegation_pgm = NULL;


  

  if (pid == FALSE) {
    dprintf (D_ALWAYS, "Failed to run myproxy-get-delegation\n");
	proxy->get_delegation_reset();
    return FALSE;
  }

  proxy->get_delegation_pid = pid;

  return TRUE;
}
