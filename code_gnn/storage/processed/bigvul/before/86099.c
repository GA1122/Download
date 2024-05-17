cupsdCloseClient(cupsd_client_t *con)	 
{
  int		partial;		 


  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Closing connection.");

  

  httpFlushWrite(con->http);

  partial = 0;

  if (con->pipe_pid != 0)
  {
    

    cupsdEndProcess(con->pipe_pid, 1);
    con->pipe_pid = 0;
  }

  if (con->file >= 0)
  {
    cupsdRemoveSelect(con->file);

    close(con->file);
    con->file = -1;
  }

  

  if (httpGetFd(con->http) >= 0)
  {
    cupsArrayRemove(ActiveClients, con);
    cupsdSetBusyState();

#ifdef HAVE_SSL
    

    if (httpIsEncrypted(con->http))
      partial = 1;
#endif  

    if (partial)
    {
      

      httpShutdown(con->http);
      cupsdAddSelect(httpGetFd(con->http), (cupsd_selfunc_t)cupsdReadClient,
                     NULL, con);

      cupsdLogClient(con, CUPSD_LOG_DEBUG, "Waiting for socket close.");
    }
    else
    {
      

      cupsdRemoveSelect(httpGetFd(con->http));
      httpClose(con->http);
      con->http = NULL;
    }
  }

  if (!partial)
  {
    

    cupsdRemoveSelect(httpGetFd(con->http));

    httpClose(con->http);

    if (con->filename)
    {
      unlink(con->filename);
      cupsdClearString(&con->filename);
    }

    cupsdClearString(&con->command);
    cupsdClearString(&con->options);
    cupsdClearString(&con->query_string);

    if (con->request)
    {
      ippDelete(con->request);
      con->request = NULL;
    }

    if (con->response)
    {
      ippDelete(con->response);
      con->response = NULL;
    }

    if (con->language)
    {
      cupsLangFree(con->language);
      con->language = NULL;
    }

#ifdef HAVE_AUTHORIZATION_H
    if (con->authref)
    {
      AuthorizationFree(con->authref, kAuthorizationFlagDefaults);
      con->authref = NULL;
    }
#endif  

    

    if (cupsArrayCount(Clients) == MaxClients)
      cupsdResumeListening();

    

    cupsArrayRemove(Clients, con);

    free(con);
  }

  return (partial);
}
