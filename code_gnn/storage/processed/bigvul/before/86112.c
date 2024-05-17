write_file(cupsd_client_t *con,		 
           http_status_t  code,		 
	   char           *filename,	 
	   char           *type,	 
	   struct stat    *filestats)	 
{
  con->file = open(filename, O_RDONLY);

  cupsdLogClient(con, CUPSD_LOG_DEBUG2, "write_file: code=%d, filename=\"%s\" (%d), type=\"%s\", filestats=%p.", code, filename, con->file, type ? type : "(null)", filestats);

  if (con->file < 0)
    return (0);

  fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);

  con->pipe_pid    = 0;
  con->sent_header = 1;

  httpClearFields(con->http);

  httpSetLength(con->http, (size_t)filestats->st_size);

  httpSetField(con->http, HTTP_FIELD_LAST_MODIFIED,
	       httpGetDateString(filestats->st_mtime));

  if (!cupsdSendHeader(con, code, type, CUPSD_AUTH_NONE))
    return (0);

  cupsdAddSelect(httpGetFd(con->http), NULL, (cupsd_selfunc_t)cupsdWriteClient, con);

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Sending file.");

  return (1);
}
