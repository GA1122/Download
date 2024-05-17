server_close_pipe (int *pipefd)	 
{
	close (pipefd[0]);	 
	close (pipefd[1]);	 
	free (pipefd);
	return FALSE;
}
