CStarter::Init( JobInfoCommunicator* my_jic, const char* original_cwd,
				bool is_gsh, int stdin_fd, int stdout_fd, 
				int stderr_fd )
{
	if( ! my_jic ) {
		EXCEPT( "CStarter::Init() called with no JobInfoCommunicator!" ); 
	}
	if( jic ) {
		delete( jic );
	}
	jic = my_jic;

	if( original_cwd ) {
		this->orig_cwd = strdup( original_cwd );
	}
	this->is_gridshell = is_gsh;
	starter_stdin_fd = stdin_fd;
	starter_stdout_fd = stdout_fd;
	starter_stderr_fd = stderr_fd;

	Config();


	if( is_gridshell ) {
		WorkingDir = Execute;
	} else {
		WorkingDir.sprintf( "%s%cdir_%ld", Execute, DIR_DELIM_CHAR, 
				 (long)daemonCore->getpid() );
	}

	daemonCore->Register_Signal(DC_SIGSUSPEND, "DC_SIGSUSPEND", 
		(SignalHandlercpp)&CStarter::RemoteSuspend, "RemoteSuspend",
		this);
	daemonCore->Register_Signal(DC_SIGCONTINUE, "DC_SIGCONTINUE",
		(SignalHandlercpp)&CStarter::RemoteContinue, "RemoteContinue",
		this);
	daemonCore->Register_Signal(DC_SIGHARDKILL, "DC_SIGHARDKILL",
		(SignalHandlercpp)&CStarter::RemoteShutdownFast, "RemoteShutdownFast",
		this);
	daemonCore->Register_Signal(DC_SIGSOFTKILL, "DC_SIGSOFTKILL",
		(SignalHandlercpp)&CStarter::RemoteShutdownGraceful, "RemoteShutdownGraceful",
		this);
	daemonCore->Register_Signal(DC_SIGPCKPT, "DC_SIGPCKPT",
		(SignalHandlercpp)&CStarter::RemotePeriodicCkpt, "RemotePeriodicCkpt",
		this);
	daemonCore->Register_Signal(DC_SIGREMOVE, "DC_SIGREMOVE",
		(SignalHandlercpp)&CStarter::RemoteRemove, "RemoteRemove",
		this);
	daemonCore->Register_Signal(SIGUSR1, "SIGUSR1",
		(SignalHandlercpp)&CStarter::RemoteRemove, "RemoteRemove",
		this);
	daemonCore->Register_Signal(DC_SIGHOLD, "DC_SIGHOLD",
		(SignalHandlercpp)&CStarter::RemoteHold, "RemoteHold",
		this);
	daemonCore->Register_Reaper("Reaper", (ReaperHandlercpp)&CStarter::Reaper,
		"Reaper", this);

	daemonCore->
		Register_Command( CA_CMD, "CA_CMD",
						  (CommandHandlercpp)&CStarter::classadCommand,
						  "CStarter::classadCommand", this, WRITE );
	daemonCore->
		Register_Command( UPDATE_GSI_CRED, "UPDATE_GSI_CRED",
						  (CommandHandlercpp)&CStarter::updateX509Proxy,
						  "CStarter::updateX509Proxy", this, WRITE );
	daemonCore->
		Register_Command( DELEGATE_GSI_CRED_STARTER,
						  "DELEGATE_GSI_CRED_STARTER",
						  (CommandHandlercpp)&CStarter::updateX509Proxy,
						  "CStarter::updateX509Proxy", this, WRITE );
	daemonCore->
		Register_Command( STARTER_HOLD_JOB,
						  "STARTER_HOLD_JOB",
						  (CommandHandlercpp)&CStarter::remoteHoldCommand,
						  "CStarter::remoteHoldCommand", this, DAEMON );
	daemonCore->
		Register_Command( CREATE_JOB_OWNER_SEC_SESSION,
						  "CREATE_JOB_OWNER_SEC_SESSION",
						  (CommandHandlercpp)&CStarter::createJobOwnerSecSession,
						  "CStarter::createJobOwnerSecSession", this, DAEMON );

	daemonCore->
		Register_Command( START_SSHD,
						  "START_SSHD",
						  (CommandHandlercpp)&CStarter::startSSHD,
						  "CStarter::startSSHD", this, READ );
	if( ! jic->init() ) {
		dprintf( D_ALWAYS, 
				 "Failed to initialize JobInfoCommunicator, aborting\n" );
		return false;
	}
	sysapi_set_resource_limits(jic->getStackSize());

	jic->setupJobEnvironment();
	return true;
}
