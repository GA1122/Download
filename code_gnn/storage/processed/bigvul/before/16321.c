display_dprintf_header(char **buf,int *bufpos,int *buflen)
{
	static pid_t mypid = 0;
	int mycluster = -1;
	int myproc = -1;

	if (!mypid) {
		mypid = daemonCore->getpid();
	}

	if (Shadow) {
		mycluster = Shadow->getCluster();
		myproc = Shadow->getProc();
	}

	if ( mycluster != -1 ) {
		return sprintf_realloc( buf, bufpos, buflen, "(%d.%d) (%ld): ", mycluster, myproc, (long)mypid );
	} else {
		return sprintf_realloc( buf, bufpos, buflen, "(?.?) (%ld): ", (long)mypid );
	}	

	return 0;
}
