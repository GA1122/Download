cgi_kill2( ClientData client_data, struct timeval* nowP )
    {
    pid_t pid;

    pid = (pid_t) client_data.i;
    if ( kill( pid, SIGKILL ) == 0 )
	syslog( LOG_ERR, "hard-killed CGI process %d", pid );
    }
