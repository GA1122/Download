cupsdReadClient(cupsd_client_t *con)	 
{
  char			line[32768],	 
			locale[64],	 
			*ptr;		 
  http_status_t		status;		 
  ipp_state_t		ipp_state;	 
  int			bytes;		 
  char			*filename;	 
  char			buf[1024];	 
  struct stat		filestats;	 
  mime_type_t		*type;		 
  cupsd_printer_t	*p;		 
  static unsigned	request_id = 0;	 


  status = HTTP_STATUS_CONTINUE;

  cupsdLogClient(con, CUPSD_LOG_DEBUG2, "cupsdReadClient: error=%d, used=%d, state=%s, data_encoding=HTTP_ENCODING_%s, data_remaining=" CUPS_LLFMT ", request=%p(%s), file=%d", httpError(con->http), (int)httpGetReady(con->http), httpStateString(httpGetState(con->http)), httpIsChunked(con->http) ? "CHUNKED" : "LENGTH", CUPS_LLCAST httpGetRemaining(con->http), con->request, con->request ? ippStateString(ippGetState(con->request)) : "", con->file);

  if (httpGetState(con->http) == HTTP_STATE_GET_SEND ||
      httpGetState(con->http) == HTTP_STATE_POST_SEND ||
      httpGetState(con->http) == HTTP_STATE_STATUS)
  {
    

    if (!httpGetReady(con->http) && recv(httpGetFd(con->http), buf, 1, MSG_PEEK) < 1)
    {
      

      cupsdLogClient(con, CUPSD_LOG_DEBUG, "Closing on EOF.");
      cupsdCloseClient(con);
      return;
    }

    cupsdLogClient(con, CUPSD_LOG_DEBUG, "Closing on unexpected HTTP read state %s.", httpStateString(httpGetState(con->http)));
    cupsdCloseClient(con);
    return;
  }

#ifdef HAVE_SSL
  if (con->auto_ssl)
  {
    

    con->auto_ssl = 0;

    if (recv(httpGetFd(con->http), buf, 1, MSG_PEEK) == 1 &&
        (!buf[0] || !strchr("DGHOPT", buf[0])))
    {
      

      cupsdLogClient(con, CUPSD_LOG_DEBUG2, "Saw first byte %02X, auto-negotiating SSL/TLS session.", buf[0] & 255);

      if (cupsd_start_tls(con, HTTP_ENCRYPTION_ALWAYS))
        cupsdCloseClient(con);

      return;
    }
  }
#endif  

  switch (httpGetState(con->http))
  {
    case HTTP_STATE_WAITING :
        

        con->operation = httpReadRequest(con->http, con->uri, sizeof(con->uri));
        if (con->operation == HTTP_STATE_ERROR ||
	    con->operation == HTTP_STATE_UNKNOWN_METHOD ||
	    con->operation == HTTP_STATE_UNKNOWN_VERSION)
	{
	  if (httpError(con->http))
	    cupsdLogClient(con, CUPSD_LOG_DEBUG,
			   "HTTP_STATE_WAITING Closing for error %d (%s)",
			   httpError(con->http), strerror(httpError(con->http)));
	  else
	    cupsdLogClient(con, CUPSD_LOG_DEBUG,
	                   "HTTP_STATE_WAITING Closing on error: %s",
			   cupsLastErrorString());

	  cupsdCloseClient(con);
	  return;
	}

        

        if (con->operation == HTTP_STATE_WAITING)
	  break;

        

	con->bytes       = 0;
	con->file        = -1;
	con->file_ready  = 0;
	con->pipe_pid    = 0;
	con->username[0] = '\0';
	con->password[0] = '\0';

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

#ifdef HAVE_GSSAPI
        con->have_gss = 0;
	con->gss_uid  = 0;
#endif  

        

        if (strcmp(con->uri, "*"))
	{
	  char	scheme[HTTP_MAX_URI],	 
		userpass[HTTP_MAX_URI],	 
		hostname[HTTP_MAX_URI],	 
		resource[HTTP_MAX_URI];	 
          int	port;			 

          

          if (httpSeparateURI(HTTP_URI_CODING_MOST, con->uri,
	                      scheme, sizeof(scheme),
	                      userpass, sizeof(userpass),
			      hostname, sizeof(hostname), &port,
			      resource, sizeof(resource)) < HTTP_URI_STATUS_OK)
          {
	    cupsdLogClient(con, CUPSD_LOG_ERROR, "Bad URI \"%s\" in request.",
                           con->uri);
	    cupsdSendError(con, HTTP_STATUS_METHOD_NOT_ALLOWED, CUPSD_AUTH_NONE);
	    cupsdCloseClient(con);
	    return;
	  }

	  

	  if (strcmp(scheme, "file") &&
	      _cups_strcasecmp(hostname, ServerName) &&
	      _cups_strcasecmp(hostname, "localhost") &&
	      !cupsArrayFind(ServerAlias, hostname) &&
	      !isdigit(hostname[0]) && hostname[0] != '[')
	  {
	    

	    cupsdLogClient(con, CUPSD_LOG_ERROR, "Bad URI \"%s\" in request.",
                           con->uri);
	    cupsdSendError(con, HTTP_STATUS_METHOD_NOT_ALLOWED, CUPSD_AUTH_NONE);
	    cupsdCloseClient(con);
	    return;
	  }

          

          strlcpy(con->uri, resource, sizeof(con->uri));
	}

        

        gettimeofday(&(con->start), NULL);

        cupsdLogClient(con, CUPSD_LOG_DEBUG, "%s %s HTTP/%d.%d",
	               httpStateString(con->operation) + 11, con->uri,
		       httpGetVersion(con->http) / 100,
                       httpGetVersion(con->http) % 100);

        if (!cupsArrayFind(ActiveClients, con))
	{
	  cupsArrayAdd(ActiveClients, con);
          cupsdSetBusyState();
        }

    case HTTP_STATE_OPTIONS :
    case HTTP_STATE_DELETE :
    case HTTP_STATE_GET :
    case HTTP_STATE_HEAD :
    case HTTP_STATE_POST :
    case HTTP_STATE_PUT :
    case HTTP_STATE_TRACE :
        

        while ((status = httpUpdate(con->http)) == HTTP_STATUS_CONTINUE)
	  if (!httpGetReady(con->http))
	    break;

	if (status != HTTP_STATUS_OK && status != HTTP_STATUS_CONTINUE)
	{
	  if (httpError(con->http) && httpError(con->http) != EPIPE)
	    cupsdLogClient(con, CUPSD_LOG_DEBUG,
                           "Closing for error %d (%s) while reading headers.",
                           httpError(con->http), strerror(httpError(con->http)));
	  else
	    cupsdLogClient(con, CUPSD_LOG_DEBUG,
	                   "Closing on EOF while reading headers.");

	  cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE);
	  cupsdCloseClient(con);
	  return;
	}
	break;

    default :
        if (!httpGetReady(con->http) && recv(httpGetFd(con->http), buf, 1, MSG_PEEK) < 1)
	{
	  

	  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Closing on EOF.");
          cupsdCloseClient(con);
	  return;
	}
        break;  
  }

  

  cupsdLogClient(con, CUPSD_LOG_DEBUG, "Read: status=%d", status);

  if (status == HTTP_STATUS_OK)
  {
    if (httpGetField(con->http, HTTP_FIELD_ACCEPT_LANGUAGE)[0])
    {
      

      if ((ptr = strchr(httpGetField(con->http, HTTP_FIELD_ACCEPT_LANGUAGE),
                        ',')) != NULL)
        *ptr = '\0';

      if ((ptr = strchr(httpGetField(con->http, HTTP_FIELD_ACCEPT_LANGUAGE),
                        ';')) != NULL)
        *ptr = '\0';

      if ((ptr = strstr(httpGetField(con->http, HTTP_FIELD_CONTENT_TYPE),
                        "charset=")) != NULL)
      {
        

        snprintf(locale, sizeof(locale), "%s.%s",
	         httpGetField(con->http, HTTP_FIELD_ACCEPT_LANGUAGE), ptr + 8);

	if ((ptr = strchr(locale, ',')) != NULL)
	  *ptr = '\0';
      }
      else
        snprintf(locale, sizeof(locale), "%s.UTF-8",
	         httpGetField(con->http, HTTP_FIELD_ACCEPT_LANGUAGE));

      con->language = cupsLangGet(locale);
    }
    else
      con->language = cupsLangGet(DefaultLocale);

    cupsdAuthorize(con);

    if (!_cups_strncasecmp(httpGetField(con->http, HTTP_FIELD_CONNECTION),
                           "Keep-Alive", 10) && KeepAlive)
      httpSetKeepAlive(con->http, HTTP_KEEPALIVE_ON);
    else if (!_cups_strncasecmp(httpGetField(con->http, HTTP_FIELD_CONNECTION),
                                "close", 5))
      httpSetKeepAlive(con->http, HTTP_KEEPALIVE_OFF);

    if (!httpGetField(con->http, HTTP_FIELD_HOST)[0] &&
        httpGetVersion(con->http) >= HTTP_VERSION_1_1)
    {
      

      if (!cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE))
      {
        cupsdLogClient(con, CUPSD_LOG_ERROR, "Missing Host: field in request.");
	cupsdCloseClient(con);
	return;
      }
    }
    else if (!valid_host(con))
    {
      

      cupsdLogClient(con, CUPSD_LOG_ERROR,
                     "Request from \"%s\" using invalid Host: field \"%s\".",
                     httpGetHostname(con->http, NULL, 0), httpGetField(con->http, HTTP_FIELD_HOST));

      if (!cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE))
      {
	cupsdCloseClient(con);
	return;
      }
    }
    else if (con->operation == HTTP_STATE_OPTIONS)
    {
      

      if (con->best && con->best->type != CUPSD_AUTH_NONE)
      {
        httpClearFields(con->http);

	if (!cupsdSendHeader(con, HTTP_STATUS_UNAUTHORIZED, NULL, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}
      }

      if (!_cups_strcasecmp(httpGetField(con->http, HTTP_FIELD_CONNECTION), "Upgrade") && strstr(httpGetField(con->http, HTTP_FIELD_UPGRADE), "TLS/") != NULL && !httpIsEncrypted(con->http))
      {
#ifdef HAVE_SSL
        

        httpClearFields(con->http);

	if (!cupsdSendHeader(con, HTTP_STATUS_SWITCHING_PROTOCOLS, NULL, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}

        if (cupsd_start_tls(con, HTTP_ENCRYPTION_REQUIRED))
        {
	  cupsdCloseClient(con);
	  return;
	}
#else
	if (!cupsdSendError(con, HTTP_STATUS_NOT_IMPLEMENTED, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}
#endif  
      }

      httpClearFields(con->http);
      httpSetField(con->http, HTTP_FIELD_ALLOW,
		   "GET, HEAD, OPTIONS, POST, PUT");
      httpSetField(con->http, HTTP_FIELD_CONTENT_LENGTH, "0");

      if (!cupsdSendHeader(con, HTTP_STATUS_OK, NULL, CUPSD_AUTH_NONE))
      {
	cupsdCloseClient(con);
	return;
      }
    }
    else if (!is_path_absolute(con->uri))
    {
      

      cupsdLogClient(con, CUPSD_LOG_ERROR,
                     "Request for non-absolute resource \"%s\".", con->uri);

      if (!cupsdSendError(con, HTTP_STATUS_FORBIDDEN, CUPSD_AUTH_NONE))
      {
	cupsdCloseClient(con);
	return;
      }
    }
    else
    {
      if (!_cups_strcasecmp(httpGetField(con->http, HTTP_FIELD_CONNECTION),
                            "Upgrade") && !httpIsEncrypted(con->http))
      {
#ifdef HAVE_SSL
        

        httpClearFields(con->http);

	if (!cupsdSendHeader(con, HTTP_STATUS_SWITCHING_PROTOCOLS, NULL,
	                     CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}

        if (cupsd_start_tls(con, HTTP_ENCRYPTION_REQUIRED))
        {
	  cupsdCloseClient(con);
	  return;
	}
#else
	if (!cupsdSendError(con, HTTP_STATUS_NOT_IMPLEMENTED, CUPSD_AUTH_NONE))
	{
	  cupsdCloseClient(con);
	  return;
	}
#endif  
      }

      if ((status = cupsdIsAuthorized(con, NULL)) != HTTP_STATUS_OK)
      {
	cupsdSendError(con, status, CUPSD_AUTH_NONE);
	cupsdCloseClient(con);
	return;
      }

      if (httpGetExpect(con->http) &&
          (con->operation == HTTP_STATE_POST || con->operation == HTTP_STATE_PUT))
      {
        if (httpGetExpect(con->http) == HTTP_STATUS_CONTINUE)
	{
	  

          if (httpWriteResponse(con->http, HTTP_STATUS_CONTINUE))
	  {
	    cupsdCloseClient(con);
	    return;
	  }
	}
	else
	{
	  

          httpClearFields(con->http);
	  httpSetField(con->http, HTTP_FIELD_CONTENT_LENGTH, "0");

	  cupsdSendError(con, HTTP_STATUS_EXPECTATION_FAILED, CUPSD_AUTH_NONE);
          cupsdCloseClient(con);
          return;
	}
      }

      switch (httpGetState(con->http))
      {
	case HTTP_STATE_GET_SEND :
            cupsdLogClient(con, CUPSD_LOG_DEBUG, "Processing GET %s", con->uri);

            if ((!strncmp(con->uri, "/ppd/", 5) ||
		 !strncmp(con->uri, "/printers/", 10) ||
		 !strncmp(con->uri, "/classes/", 9)) &&
		!strcmp(con->uri + strlen(con->uri) - 4, ".ppd"))
	    {
	      

              con->uri[strlen(con->uri) - 4] = '\0';	 

	      if (!strncmp(con->uri, "/ppd/", 5))
		p = cupsdFindPrinter(con->uri + 5);
	      else if (!strncmp(con->uri, "/printers/", 10))
		p = cupsdFindPrinter(con->uri + 10);
	      else
	      {
		p = cupsdFindClass(con->uri + 9);

		if (p)
		{
		  int i;		 

		  for (i = 0; i < p->num_printers; i ++)
		  {
		    if (!(p->printers[i]->type & CUPS_PRINTER_CLASS))
		    {
		      char ppdname[1024]; 

		      snprintf(ppdname, sizeof(ppdname), "%s/ppd/%s.ppd",
		               ServerRoot, p->printers[i]->name);
		      if (!access(ppdname, 0))
		      {
		        p = p->printers[i];
		        break;
		      }
		    }
		  }

                  if (i >= p->num_printers)
                    p = NULL;
		}
	      }

	      if (p)
	      {
		snprintf(con->uri, sizeof(con->uri), "/ppd/%s.ppd", p->name);
	      }
	      else
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }
	    }
            else if ((!strncmp(con->uri, "/icons/", 7) ||
		      !strncmp(con->uri, "/printers/", 10) ||
		      !strncmp(con->uri, "/classes/", 9)) &&
		     !strcmp(con->uri + strlen(con->uri) - 4, ".png"))
	    {
	      

	      con->uri[strlen(con->uri) - 4] = '\0';	 

              if (!strncmp(con->uri, "/icons/", 7))
                p = cupsdFindPrinter(con->uri + 7);
              else if (!strncmp(con->uri, "/printers/", 10))
                p = cupsdFindPrinter(con->uri + 10);
              else
              {
		p = cupsdFindClass(con->uri + 9);

		if (p)
		{
		  int i;		 

		  for (i = 0; i < p->num_printers; i ++)
		  {
		    if (!(p->printers[i]->type & CUPS_PRINTER_CLASS))
		    {
		      char ppdname[1024]; 

		      snprintf(ppdname, sizeof(ppdname), "%s/ppd/%s.ppd",
		               ServerRoot, p->printers[i]->name);
		      if (!access(ppdname, 0))
		      {
		        p = p->printers[i];
		        break;
		      }
		    }
		  }

                  if (i >= p->num_printers)
                    p = NULL;
		}
	      }

              if (p)
		snprintf(con->uri, sizeof(con->uri), "/icons/%s.png", p->name);
	      else
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }
	    }

	    if ((!strncmp(con->uri, "/admin", 6) && strcmp(con->uri, "/admin/conf/cupsd.conf") && strncmp(con->uri, "/admin/log/", 11)) ||
		 !strncmp(con->uri, "/printers", 9) ||
		 !strncmp(con->uri, "/classes", 8) ||
		 !strncmp(con->uri, "/help", 5) ||
		 !strncmp(con->uri, "/jobs", 5))
	    {
	      if (!WebInterface)
	      {
	        

		if (!cupsdSendError(con, HTTP_STATUS_CUPS_WEBIF_DISABLED, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }

	      

              if (!strncmp(con->uri, "/admin", 6))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/admin.cgi",
		                ServerBin);

		cupsdSetString(&con->options, strchr(con->uri + 6, '?'));
	      }
              else if (!strncmp(con->uri, "/printers", 9))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/printers.cgi",
		                ServerBin);

                if (con->uri[9] && con->uri[10])
		  cupsdSetString(&con->options, con->uri + 9);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/classes", 8))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/classes.cgi",
		                ServerBin);

                if (con->uri[8] && con->uri[9])
		  cupsdSetString(&con->options, con->uri + 8);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/jobs", 5))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/jobs.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/help.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }

              if (!cupsdSendCommand(con, con->command, con->options, 0))
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
              }
	      else
        	cupsdLogRequest(con, HTTP_STATUS_OK);

	      if (httpGetVersion(con->http) <= HTTP_VERSION_1_0)
		httpSetKeepAlive(con->http, HTTP_KEEPALIVE_OFF);
	    }
            else if (!strncmp(con->uri, "/admin/log/", 11) && (strchr(con->uri + 11, '/') || strlen(con->uri) == 11))
	    {
	      

	      cupsdLogClient(con, CUPSD_LOG_ERROR, "Request for subdirectory \"%s\".", con->uri);

	      if (!cupsdSendError(con, HTTP_STATUS_FORBIDDEN, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }
	    else
	    {
	      

              if ((filename = get_file(con, &filestats, buf,
	                               sizeof(buf))) == NULL)
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }

	      type = mimeFileType(MimeDatabase, filename, NULL, NULL);

              cupsdLogClient(con, CUPSD_LOG_DEBUG, "filename=\"%s\", type=%s/%s", filename, type ? type->super : "", type ? type->type : "");

              if (is_cgi(con, filename, &filestats, type))
	      {
	        

        	if (!cupsdSendCommand(con, con->command, con->options, 0))
		{
		  if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		  {
		    cupsdCloseClient(con);
		    return;
		  }
        	}
		else
        	  cupsdLogRequest(con, HTTP_STATUS_OK);

		if (httpGetVersion(con->http) <= HTTP_VERSION_1_0)
		  httpSetKeepAlive(con->http, HTTP_KEEPALIVE_OFF);
	        break;
	      }

	      if (!check_if_modified(con, &filestats))
              {
        	if (!cupsdSendError(con, HTTP_STATUS_NOT_MODIFIED, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	      else
              {
		if (type == NULL)
	          strlcpy(line, "text/plain", sizeof(line));
		else
	          snprintf(line, sizeof(line), "%s/%s", type->super, type->type);

        	if (!write_file(con, HTTP_STATUS_OK, filename, line, &filestats))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	    }
            break;

	case HTTP_STATE_POST_RECV :
            

            if (httpGetField(con->http, HTTP_FIELD_CONTENT_LENGTH)[0] &&
		MaxRequestSize > 0 &&
		httpGetLength2(con->http) > MaxRequestSize)
	    {
	      

              if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
            }
	    else if (httpGetLength2(con->http) < 0)
	    {
	      

              if (!cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }

            

	    if (!strcmp(httpGetField(con->http, HTTP_FIELD_CONTENT_TYPE),
	                "application/ipp"))
              con->request = ippNew();
            else if (!WebInterface)
	    {
	      

	      if (!cupsdSendError(con, HTTP_STATUS_CUPS_WEBIF_DISABLED, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }
	    else if ((!strncmp(con->uri, "/admin", 6) && strncmp(con->uri, "/admin/log/", 11)) ||
	             !strncmp(con->uri, "/printers", 9) ||
	             !strncmp(con->uri, "/classes", 8) ||
	             !strncmp(con->uri, "/help", 5) ||
	             !strncmp(con->uri, "/jobs", 5))
	    {
	      

              if (!strncmp(con->uri, "/admin", 6))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/admin.cgi",
		                ServerBin);

		cupsdSetString(&con->options, strchr(con->uri + 6, '?'));
	      }
              else if (!strncmp(con->uri, "/printers", 9))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/printers.cgi",
		                ServerBin);

                if (con->uri[9] && con->uri[10])
		  cupsdSetString(&con->options, con->uri + 9);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/classes", 8))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/classes.cgi",
		                ServerBin);

                if (con->uri[8] && con->uri[9])
		  cupsdSetString(&con->options, con->uri + 8);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else if (!strncmp(con->uri, "/jobs", 5))
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/jobs.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }
	      else
	      {
		cupsdSetStringf(&con->command, "%s/cgi-bin/help.cgi",
		                ServerBin);

                if (con->uri[5] && con->uri[6])
		  cupsdSetString(&con->options, con->uri + 5);
		else
		  cupsdSetString(&con->options, NULL);
	      }

	      if (httpGetVersion(con->http) <= HTTP_VERSION_1_0)
		httpSetKeepAlive(con->http, HTTP_KEEPALIVE_OFF);
	    }
	    else
	    {
	      

              if ((filename = get_file(con, &filestats, buf,
	                               sizeof(buf))) == NULL)
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		break;
	      }

	      type = mimeFileType(MimeDatabase, filename, NULL, NULL);

              if (!is_cgi(con, filename, &filestats, type))
	      {
	        

		if (!cupsdSendError(con, HTTP_STATUS_UNAUTHORIZED, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	    }
	    break;

	case HTTP_STATE_PUT_RECV :
	    

            if (strcmp(con->uri, "/admin/conf/cupsd.conf"))
	    {
	      

	      cupsdLogClient(con, CUPSD_LOG_ERROR,
			     "Disallowed PUT request for \"%s\".", con->uri);

	      if (!cupsdSendError(con, HTTP_STATUS_FORBIDDEN, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }

            

            if (httpGetField(con->http, HTTP_FIELD_CONTENT_LENGTH)[0] &&
		MaxRequestSize > 0 &&
		httpGetLength2(con->http) > MaxRequestSize)
	    {
	      

              if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
            }
	    else if (httpGetLength2(con->http) < 0)
	    {
	      

              if (!cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

	      break;
	    }

            

            cupsdSetStringf(&con->filename, "%s/%08x", RequestRoot,
	                    request_id ++);
	    con->file = open(con->filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);

	    if (con->file < 0)
	    {
	      cupsdLogClient(con, CUPSD_LOG_ERROR,
	                     "Unable to create request file \"%s\": %s",
                             con->filename, strerror(errno));

	      if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }

	    fchmod(con->file, 0640);
	    fchown(con->file, RunUser, Group);
	    fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);
	    break;

	case HTTP_STATE_DELETE :
	case HTTP_STATE_TRACE :
            cupsdSendError(con, HTTP_STATUS_NOT_IMPLEMENTED, CUPSD_AUTH_NONE);
	    cupsdCloseClient(con);
	    return;

	case HTTP_STATE_HEAD :
            if (!strncmp(con->uri, "/printers/", 10) &&
		!strcmp(con->uri + strlen(con->uri) - 4, ".ppd"))
	    {
	      

              con->uri[strlen(con->uri) - 4] = '\0';	 

              if ((p = cupsdFindPrinter(con->uri + 10)) != NULL)
		snprintf(con->uri, sizeof(con->uri), "/ppd/%s.ppd", p->name);
	      else
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		cupsdLogRequest(con, HTTP_STATUS_NOT_FOUND);
		break;
	      }
	    }
            else if (!strncmp(con->uri, "/printers/", 10) &&
		     !strcmp(con->uri + strlen(con->uri) - 4, ".png"))
	    {
	      

              con->uri[strlen(con->uri) - 4] = '\0';	 

              if ((p = cupsdFindPrinter(con->uri + 10)) != NULL)
		snprintf(con->uri, sizeof(con->uri), "/icons/%s.png", p->name);
	      else
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}

		cupsdLogRequest(con, HTTP_STATUS_NOT_FOUND);
		break;
	      }
	    }
	    else if (!WebInterface)
	    {
              httpClearFields(con->http);

              if (!cupsdSendHeader(con, HTTP_STATUS_OK, NULL, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

              cupsdLogRequest(con, HTTP_STATUS_OK);
	      break;
	    }

	    if ((!strncmp(con->uri, "/admin", 6) && strcmp(con->uri, "/admin/conf/cupsd.conf") && strncmp(con->uri, "/admin/log/", 11)) ||
		!strncmp(con->uri, "/printers", 9) ||
		!strncmp(con->uri, "/classes", 8) ||
		!strncmp(con->uri, "/help", 5) ||
		!strncmp(con->uri, "/jobs", 5))
	    {
	      

              httpClearFields(con->http);

              if (!cupsdSendHeader(con, HTTP_STATUS_OK, "text/html", CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

              cupsdLogRequest(con, HTTP_STATUS_OK);
	    }
            else if (!strncmp(con->uri, "/admin/log/", 11) && (strchr(con->uri + 11, '/') || strlen(con->uri) == 11))
	    {
	      

	      cupsdLogClient(con, CUPSD_LOG_ERROR,
			     "Request for subdirectory \"%s\".", con->uri);

	      if (!cupsdSendError(con, HTTP_STATUS_FORBIDDEN, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

              cupsdLogRequest(con, HTTP_STATUS_FORBIDDEN);
	      break;
	    }
	    else if ((filename = get_file(con, &filestats, buf,
	                                  sizeof(buf))) == NULL)
	    {
              httpClearFields(con->http);

	      if (!cupsdSendHeader(con, HTTP_STATUS_NOT_FOUND, "text/html",
	                           CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

              cupsdLogRequest(con, HTTP_STATUS_NOT_FOUND);
	    }
	    else if (!check_if_modified(con, &filestats))
            {
              if (!cupsdSendError(con, HTTP_STATUS_NOT_MODIFIED, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

              cupsdLogRequest(con, HTTP_STATUS_NOT_MODIFIED);
	    }
	    else
	    {
	      

	      type = mimeFileType(MimeDatabase, filename, NULL, NULL);
	      if (type == NULL)
		strlcpy(line, "text/plain", sizeof(line));
	      else
		snprintf(line, sizeof(line), "%s/%s", type->super, type->type);

              httpClearFields(con->http);

	      httpSetField(con->http, HTTP_FIELD_LAST_MODIFIED,
			   httpGetDateString(filestats.st_mtime));
	      httpSetLength(con->http, (size_t)filestats.st_size);

              if (!cupsdSendHeader(con, HTTP_STATUS_OK, line, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }

              cupsdLogRequest(con, HTTP_STATUS_OK);
	    }
            break;

	default :
            break;  
      }
    }
  }

  

  switch (httpGetState(con->http))
  {
    case HTTP_STATE_PUT_RECV :
        do
	{
          if ((bytes = httpRead2(con->http, line, sizeof(line))) < 0)
	  {
	    if (httpError(con->http) && httpError(con->http) != EPIPE)
	      cupsdLogClient(con, CUPSD_LOG_DEBUG,
                             "HTTP_STATE_PUT_RECV Closing for error %d (%s)",
                             httpError(con->http), strerror(httpError(con->http)));
	    else
	      cupsdLogClient(con, CUPSD_LOG_DEBUG,
			     "HTTP_STATE_PUT_RECV Closing on EOF.");

	    cupsdCloseClient(con);
	    return;
	  }
	  else if (bytes > 0)
	  {
	    con->bytes += bytes;

            if (MaxRequestSize > 0 && con->bytes > MaxRequestSize)
            {
	      close(con->file);
	      con->file = -1;
	      unlink(con->filename);
	      cupsdClearString(&con->filename);

              if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
            }

            if (write(con->file, line, (size_t)bytes) < bytes)
	    {
              cupsdLogClient(con, CUPSD_LOG_ERROR,
	                     "Unable to write %d bytes to \"%s\": %s", bytes,
                             con->filename, strerror(errno));

	      close(con->file);
	      con->file = -1;
	      unlink(con->filename);
	      cupsdClearString(&con->filename);

              if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }
	  }
          else if (httpGetState(con->http) == HTTP_STATE_PUT_RECV)
          {
            cupsdCloseClient(con);
            return;
          }
        }
	while (httpGetState(con->http) == HTTP_STATE_PUT_RECV && httpGetReady(con->http));

        if (httpGetState(con->http) == HTTP_STATE_STATUS)
	{
	  

	  fstat(con->file, &filestats);

	  close(con->file);
	  con->file = -1;

          if (filestats.st_size > MaxRequestSize &&
	      MaxRequestSize > 0)
	  {
	    

	    unlink(con->filename);
	    cupsdClearString(&con->filename);

            if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	    {
	      cupsdCloseClient(con);
	      return;
	    }
	  }

          

          status = install_cupsd_conf(con);

          

          if (!cupsdSendError(con, status, CUPSD_AUTH_NONE))
	  {
	    cupsdCloseClient(con);
	    return;
	  }
	}
        break;

    case HTTP_STATE_POST_RECV :
        do
	{
          if (con->request && con->file < 0)
	  {
	    

	    if (!httpWait(con->http, 0))
	      return;

	    if ((ipp_state = ippRead(con->http, con->request)) == IPP_STATE_ERROR)
	    {
              cupsdLogClient(con, CUPSD_LOG_ERROR, "IPP read error: %s",
                             cupsLastErrorString());

	      cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE);
	      cupsdCloseClient(con);
	      return;
	    }
	    else if (ipp_state != IPP_STATE_DATA)
	    {
              if (httpGetState(con->http) == HTTP_STATE_POST_SEND)
	      {
		cupsdSendError(con, HTTP_STATUS_BAD_REQUEST, CUPSD_AUTH_NONE);
		cupsdCloseClient(con);
		return;
	      }

	      if (httpGetReady(con->http))
	        continue;
	      break;
            }
	    else
	    {
	      cupsdLogClient(con, CUPSD_LOG_DEBUG, "%d.%d %s %d",
			      con->request->request.op.version[0],
			      con->request->request.op.version[1],
			      ippOpString(con->request->request.op.operation_id),
			      con->request->request.op.request_id);
	      con->bytes += (off_t)ippLength(con->request);
	    }
	  }

          if (con->file < 0 && httpGetState(con->http) != HTTP_STATE_POST_SEND)
	  {
            

            cupsdSetStringf(&con->filename, "%s/%08x", RequestRoot,
	                    request_id ++);
	    con->file = open(con->filename, O_WRONLY | O_CREAT | O_TRUNC, 0640);

	    if (con->file < 0)
	    {
	      cupsdLogClient(con, CUPSD_LOG_ERROR,
	                     "Unable to create request file \"%s\": %s",
                             con->filename, strerror(errno));

	      if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }

	    fchmod(con->file, 0640);
	    fchown(con->file, RunUser, Group);
            fcntl(con->file, F_SETFD, fcntl(con->file, F_GETFD) | FD_CLOEXEC);
	  }

	  if (httpGetState(con->http) != HTTP_STATE_POST_SEND)
	  {
	    if (!httpWait(con->http, 0))
	      return;
            else if ((bytes = httpRead2(con->http, line, sizeof(line))) < 0)
	    {
	      if (httpError(con->http) && httpError(con->http) != EPIPE)
		cupsdLogClient(con, CUPSD_LOG_DEBUG,
			       "HTTP_STATE_POST_SEND Closing for error %d (%s)",
                               httpError(con->http), strerror(httpError(con->http)));
	      else
		cupsdLogClient(con, CUPSD_LOG_DEBUG,
			       "HTTP_STATE_POST_SEND Closing on EOF.");

	      cupsdCloseClient(con);
	      return;
	    }
	    else if (bytes > 0)
	    {
	      con->bytes += bytes;

              if (MaxRequestSize > 0 && con->bytes > MaxRequestSize)
              {
                close(con->file);
                con->file = -1;
                unlink(con->filename);
                cupsdClearString(&con->filename);

                if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
                {
                  cupsdCloseClient(con);
                  return;
                }
              }

              if (write(con->file, line, (size_t)bytes) < bytes)
	      {
        	cupsdLogClient(con, CUPSD_LOG_ERROR,
	                       "Unable to write %d bytes to \"%s\": %s",
                               bytes, con->filename, strerror(errno));

		close(con->file);
		con->file = -1;
		unlink(con->filename);
		cupsdClearString(&con->filename);

        	if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE,
		                    CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
	      }
	    }
	    else if (httpGetState(con->http) == HTTP_STATE_POST_RECV)
              return;
	    else if (httpGetState(con->http) != HTTP_STATE_POST_SEND)
	    {
	      cupsdLogClient(con, CUPSD_LOG_DEBUG,
	                     "Closing on unexpected state %s.",
			     httpStateString(httpGetState(con->http)));
	      cupsdCloseClient(con);
	      return;
	    }
	  }
        }
	while (httpGetState(con->http) == HTTP_STATE_POST_RECV && httpGetReady(con->http));

	if (httpGetState(con->http) == HTTP_STATE_POST_SEND)
	{
	  if (con->file >= 0)
	  {
	    fstat(con->file, &filestats);

	    close(con->file);
	    con->file = -1;

            if (filestats.st_size > MaxRequestSize &&
	        MaxRequestSize > 0)
	    {
	      

	      unlink(con->filename);
	      cupsdClearString(&con->filename);

	      if (con->request)
	      {
	        

	        ippDelete(con->request);
		con->request = NULL;
              }

              if (!cupsdSendError(con, HTTP_STATUS_REQUEST_TOO_LARGE, CUPSD_AUTH_NONE))
	      {
		cupsdCloseClient(con);
		return;
	      }
	    }
	    else if (filestats.st_size == 0)
	    {
	      

	      unlink(con->filename);
	      cupsdClearString(&con->filename);
	    }

	    if (con->command)
	    {
	      if (!cupsdSendCommand(con, con->command, con->options, 0))
	      {
		if (!cupsdSendError(con, HTTP_STATUS_NOT_FOUND, CUPSD_AUTH_NONE))
		{
		  cupsdCloseClient(con);
		  return;
		}
              }
	      else
        	cupsdLogRequest(con, HTTP_STATUS_OK);
            }
	  }

          if (con->request)
	  {
	    cupsdProcessIPPRequest(con);

	    if (con->filename)
	    {
	      unlink(con->filename);
	      cupsdClearString(&con->filename);
	    }

	    return;
	  }
	}
        break;

    default :
        break;  
  }

  if (httpGetState(con->http) == HTTP_STATE_WAITING)
  {
    if (!httpGetKeepAlive(con->http))
    {
      cupsdLogClient(con, CUPSD_LOG_DEBUG,
                     "Closing because Keep-Alive is disabled.");
      cupsdCloseClient(con);
    }
    else
    {
      cupsArrayRemove(ActiveClients, con);
      cupsdSetBusyState();
    }
  }
}