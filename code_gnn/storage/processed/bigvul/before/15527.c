control_client_sighandler(int signo)
{
	muxclient_terminate = signo;
}
