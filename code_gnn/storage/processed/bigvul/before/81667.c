fclose_on_exec(struct mg_file_access *filep, struct mg_connection *conn)
{
	if (filep != NULL && filep->fp != NULL) {
#if defined(_WIN32)
		(void)conn;  
#else
		if (fcntl(fileno(filep->fp), F_SETFD, FD_CLOEXEC) != 0) {
			mg_cry_internal(conn,
			                "%s: fcntl(F_SETFD FD_CLOEXEC) failed: %s",
			                __func__,
			                strerror(ERRNO));
		}
#endif
	}
}