cupsdUpdateCGI(void)
{
  char		*ptr,			 
		message[1024];		 
  int		loglevel;		 


  while ((ptr = cupsdStatBufUpdate(CGIStatusBuffer, &loglevel,
                                   message, sizeof(message))) != NULL)
  {
    if (loglevel == CUPSD_LOG_INFO)
      cupsdLogMessage(CUPSD_LOG_INFO, "%s", message);

    if (!strchr(CGIStatusBuffer->buffer, '\n'))
      break;
  }

  if (ptr == NULL && !CGIStatusBuffer->bufused)
  {
    

    cupsdLogMessage(CUPSD_LOG_CRIT,
                    "cupsdUpdateCGI: error reading from CGI error pipe - %s",
                    strerror(errno));
  }
}