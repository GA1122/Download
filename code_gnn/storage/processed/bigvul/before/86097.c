cupsdAcceptClient(cupsd_listener_t *lis) 
{
  const char		*hostname;	 
  char			name[256];	 
  int			count;		 
  cupsd_client_t	*con,		 
			*tempcon;	 
  socklen_t		addrlen;	 
  http_addr_t		temp;		 
  static time_t		last_dos = 0;	 
#ifdef HAVE_TCPD_H
  struct request_info	wrap_req;	 
#endif  


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cupsdAcceptClient(lis=%p(%d)) Clients=%d", lis, lis->fd, cupsArrayCount(Clients));

  

  if (cupsArrayCount(Clients) == MaxClients)
    return;

  

  if (!Clients)
    Clients = cupsArrayNew(NULL, NULL);

  if (!Clients)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to allocate memory for clients array!");
    cupsdPauseListening();
    return;
  }

  if (!ActiveClients)
    ActiveClients = cupsArrayNew((cups_array_func_t)compare_clients, NULL);

  if (!ActiveClients)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR,
                    "Unable to allocate memory for active clients array!");
    cupsdPauseListening();
    return;
  }

  if ((con = calloc(1, sizeof(cupsd_client_t))) == NULL)
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to allocate memory for client!");
    cupsdPauseListening();
    return;
  }

  

  con->number = ++ LastClientNumber;
  con->file   = -1;

  if ((con->http = httpAcceptConnection(lis->fd, 0)) == NULL)
  {
    if (errno == ENFILE || errno == EMFILE)
      cupsdPauseListening();

    cupsdLogMessage(CUPSD_LOG_ERROR, "Unable to accept client connection - %s.",
                    strerror(errno));
    free(con);

    return;
  }

  

  addrlen = sizeof(con->clientaddr);

  if (getsockname(httpGetFd(con->http), (struct sockaddr *)&con->clientaddr, &addrlen) || addrlen == 0)
    con->clientaddr = lis->address;

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Server address is \"%s\".", httpAddrString(&con->clientaddr, name, sizeof(name)));

  

  for (count = 0, tempcon = (cupsd_client_t *)cupsArrayFirst(Clients);
       tempcon;
       tempcon = (cupsd_client_t *)cupsArrayNext(Clients))
    if (httpAddrEqual(httpGetAddress(tempcon->http), httpGetAddress(con->http)))
    {
      count ++;
      if (count >= MaxClientsPerHost)
	break;
    }

  if (count >= MaxClientsPerHost)
  {
    if ((time(NULL) - last_dos) >= 60)
    {
      last_dos = time(NULL);
      cupsdLogMessage(CUPSD_LOG_WARN,
                      "Possible DoS attack - more than %d clients connecting "
		      "from %s.",
	              MaxClientsPerHost,
		      httpGetHostname(con->http, name, sizeof(name)));
    }

    httpClose(con->http);
    free(con);
    return;
  }

  

  if (HostNameLookups)
    hostname = httpResolveHostname(con->http, NULL, 0);
  else
    hostname = httpGetHostname(con->http, NULL, 0);

  if (hostname == NULL && HostNameLookups == 2)
  {
    

    httpClose(con->http);

    cupsdLogClient(con, CUPSD_LOG_WARN,
                    "Name lookup failed - connection from %s closed!",
                    httpGetHostname(con->http, NULL, 0));

    free(con);
    return;
  }

  if (HostNameLookups == 2)
  {
    

    http_addrlist_t	*addrlist,	 
			*addr;		 

    if ((addrlist = httpAddrGetList(hostname, AF_UNSPEC, NULL)) != NULL)
    {
      

      for (addr = addrlist; addr; addr = addr->next)
        if (httpAddrEqual(httpGetAddress(con->http), &(addr->addr)))
          break;
    }
    else
      addr = NULL;

    httpAddrFreeList(addrlist);

    if (!addr)
    {
      

      httpClose(con->http);

      cupsdLogClient(con, CUPSD_LOG_WARN,
                      "IP lookup failed - connection from %s closed!",
                      httpGetHostname(con->http, NULL, 0));
      free(con);
      return;
    }
  }

#ifdef HAVE_TCPD_H
  

  request_init(&wrap_req, RQ_DAEMON, "cupsd", RQ_FILE, httpGetFd(con->http),
               NULL);
  fromhost(&wrap_req);

  if (!hosts_access(&wrap_req))
  {
    httpClose(con->http);

    cupsdLogClient(con, CUPSD_LOG_WARN,
                    "Connection from %s refused by /etc/hosts.allow and "
		    "/etc/hosts.deny rules.", httpGetHostname(con->http, NULL, 0));
    free(con);
    return;
  }
#endif  

#ifdef AF_LOCAL
  if (httpAddrFamily(httpGetAddress(con->http)) == AF_LOCAL)
  {
#  ifdef __APPLE__
    socklen_t	peersize;		 
    pid_t	peerpid;		 
    char	peername[256];		 

    peersize = sizeof(peerpid);
    if (!getsockopt(httpGetFd(con->http), SOL_LOCAL, LOCAL_PEERPID, &peerpid,
                    &peersize))
    {
      if (!proc_name((int)peerpid, peername, sizeof(peername)))
	cupsdLogClient(con, CUPSD_LOG_DEBUG,
	               "Accepted from %s (Domain ???[%d])",
                       httpGetHostname(con->http, NULL, 0), (int)peerpid);
      else
	cupsdLogClient(con, CUPSD_LOG_DEBUG,
                       "Accepted from %s (Domain %s[%d])",
                       httpGetHostname(con->http, NULL, 0), peername, (int)peerpid);
    }
    else
#  endif  

    cupsdLogClient(con, CUPSD_LOG_DEBUG, "Accepted from %s (Domain)",
                   httpGetHostname(con->http, NULL, 0));
  }
  else
#endif  
  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Accepted from %s:%d (IPv%d)",
                 httpGetHostname(con->http, NULL, 0),
		 httpAddrPort(httpGetAddress(con->http)),
		 httpAddrFamily(httpGetAddress(con->http)) == AF_INET ? 4 : 6);

  

  addrlen = sizeof(temp);
  if (getsockname(httpGetFd(con->http), (struct sockaddr *)&temp, &addrlen))
  {
    cupsdLogClient(con, CUPSD_LOG_ERROR, "Unable to get local address - %s",
                   strerror(errno));

    strlcpy(con->servername, "localhost", sizeof(con->servername));
    con->serverport = LocalPort;
  }
#ifdef AF_LOCAL
  else if (httpAddrFamily(&temp) == AF_LOCAL)
  {
    strlcpy(con->servername, "localhost", sizeof(con->servername));
    con->serverport = LocalPort;
  }
#endif  
  else
  {
    if (httpAddrLocalhost(&temp))
      strlcpy(con->servername, "localhost", sizeof(con->servername));
    else if (HostNameLookups)
      httpAddrLookup(&temp, con->servername, sizeof(con->servername));
    else
      httpAddrString(&temp, con->servername, sizeof(con->servername));

    con->serverport = httpAddrPort(&(lis->address));
  }

  

  cupsArrayAdd(Clients, con);

  

  cupsdAddSelect(httpGetFd(con->http), (cupsd_selfunc_t)cupsdReadClient, NULL,
                 con);

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Waiting for request.");

  

  if (cupsArrayCount(Clients) == MaxClients)
    cupsdPauseListening();

#ifdef HAVE_SSL
  

  if (lis->encryption == HTTP_ENCRYPTION_ALWAYS)
  {
    

    if (cupsd_start_tls(con, HTTP_ENCRYPTION_ALWAYS))
      cupsdCloseClient(con);
  }
  else
    con->auto_ssl = 1;
#endif  
}
