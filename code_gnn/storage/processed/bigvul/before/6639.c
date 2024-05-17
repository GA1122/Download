AviaryScheddPlugin::earlyInitialize()
{

	static bool skip = false;
	if (skip) return; skip = true;

    string log_name;
    sprintf(log_name,"aviary_job.log");
	string myname = "job@" + getScheddName();
    provider = AviaryProviderFactory::create(log_name,myname,
											 "SCHEDULER","JOB","services/job/");
    if (!provider) {
        EXCEPT("Unable to configure AviaryProvider. Exiting...");
    }

	schedulerObj = SchedulerObject::getInstance();

	dirtyJobs = new DirtyJobsType();

	isHandlerRegistered = false;

	ReliSock *sock = new ReliSock;
	if (!sock) {
		EXCEPT("Failed to allocate transport socket");
	}
	if (!sock->assign(provider->getListenerSocket())) {
		EXCEPT("Failed to bind transport socket");
	}
	int index;
	if (-1 == (index =
			   daemonCore->Register_Socket((Stream *) sock,
										   "Aviary Method Socket",
										   (SocketHandlercpp) ( &AviaryScheddPlugin::handleTransportSocket ),
										   "Handler for Aviary Methods.",
										   this))) {
		EXCEPT("Failed to register transport socket");
	}

	m_initialized = false;
}
