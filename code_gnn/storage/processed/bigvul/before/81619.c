abort_process(void *data)
{
	 
	pid_t pid = (pid_t)data;
	int status = 0;
	pid_t rpid = waitpid(pid, &status, WNOHANG);
	if ((rpid != (pid_t)-1) && (status == 0)) {
		 
		DEBUG_TRACE("CGI timer: Stop child process %p\n", pid);
		kill(pid, SIGABRT);

		 
		while (waitpid(pid, &status, 0) != (pid_t)-1)  
			;
	} else {
		DEBUG_TRACE("CGI timer: Child process %p already stopped in time\n",
		            pid);
	}
	return 0;
}
