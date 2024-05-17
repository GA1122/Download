pipe_command(cupsd_client_t *con,	 
             int            infile,	 
             int            *outfile,	 
	     char           *command,	 
	     char           *options,	 
	     int            root)	 
{
  int		i;			 
  int		pid;			 
  char		*commptr,		 
		commch;			 
  char		*uriptr;		 
  int		fds[2];			 
  int		argc;			 
  int		envc;			 
  char		argbuf[10240],		 
		*argv[100],		 
		*envp[MAX_ENV + 20];	 
  char		auth_type[256],		 
		content_length[1024],	 
		content_type[1024],	 
		http_cookie[32768],	 
		http_referer[1024],	 
		http_user_agent[1024],	 
		lang[1024],		 
		path_info[1024],	 
		remote_addr[1024],	 
		remote_host[1024],	 
		remote_user[1024],	 
		script_filename[1024],	 
		script_name[1024],	 
		server_name[1024],	 
		server_port[1024];	 
  ipp_attribute_t *attr;		 


  

  cupsdLogClient(con, CUPSD_LOG_DEBUG2, "pipe_command: infile=%d, outfile=%p, command=\"%s\", options=\"%s\", root=%d", infile, outfile, command, options ? options : "(null)", root);

  argv[0] = command;

  if (options)
    strlcpy(argbuf, options, sizeof(argbuf));
  else
    argbuf[0] = '\0';

  if (argbuf[0] == '/')
  {
    

    if ((commptr = strchr(argbuf, '?')) == NULL)
      commptr = argbuf + strlen(argbuf);

    commch   = *commptr;
    *commptr = '\0';
    snprintf(path_info, sizeof(path_info), "PATH_INFO=%s", argbuf);
    *commptr = commch;
  }
  else
  {
    commptr      = argbuf;
    path_info[0] = '\0';

    if (*commptr == ' ')
      commptr ++;
  }

  if (*commptr == '?' && con->operation == HTTP_STATE_GET && !con->query_string)
  {
    commptr ++;
    cupsdSetStringf(&(con->query_string), "QUERY_STRING=%s", commptr);
  }

  argc = 1;

  if (*commptr)
  {
    argv[argc ++] = commptr;

    for (; *commptr && argc < 99; commptr ++)
    {
      

      if (*commptr == ' ' || *commptr == '+')
      {
	while (*commptr == ' ' || *commptr == '+')
	  *commptr++ = '\0';

        

	if (*commptr)
	{
	  argv[argc] = commptr;
	  argc ++;
	}
	else
	  break;
      }
      else if (*commptr == '%' && isxdigit(commptr[1] & 255) &&
               isxdigit(commptr[2] & 255))
      {
        

	if (commptr[1] >= '0' && commptr[1] <= '9')
          *commptr = (char)((commptr[1] - '0') << 4);
	else
          *commptr = (char)((tolower(commptr[1]) - 'a' + 10) << 4);

	if (commptr[2] >= '0' && commptr[2] <= '9')
          *commptr |= commptr[2] - '0';
	else
          *commptr |= tolower(commptr[2]) - 'a' + 10;

	_cups_strcpy(commptr + 1, commptr + 3);

        

	if (!*commptr)
          break;
      }
    }
  }

  argv[argc] = NULL;

  

  if (con->username[0])
  {
    snprintf(auth_type, sizeof(auth_type), "AUTH_TYPE=%s",
             httpGetField(con->http, HTTP_FIELD_AUTHORIZATION));

    if ((uriptr = strchr(auth_type + 10, ' ')) != NULL)
      *uriptr = '\0';
  }
  else
    auth_type[0] = '\0';

  if (con->request &&
      (attr = ippFindAttribute(con->request, "attributes-natural-language",
                               IPP_TAG_LANGUAGE)) != NULL)
  {
    switch (strlen(attr->values[0].string.text))
    {
      default :
	  

	  strlcpy(lang, "LANG=C", sizeof(lang));
	  break;

      case 2 :
	  

	  snprintf(lang, sizeof(lang), "LANG=%s.UTF8",
		   attr->values[0].string.text);
	  break;

      case 5 :
	  

	  snprintf(lang, sizeof(lang), "LANG=%c%c_%c%c.UTF8",
		   attr->values[0].string.text[0],
		   attr->values[0].string.text[1],
		   toupper(attr->values[0].string.text[3] & 255),
		   toupper(attr->values[0].string.text[4] & 255));
	  break;
    }
  }
  else if (con->language)
    snprintf(lang, sizeof(lang), "LANG=%s.UTF8", con->language->language);
  else
    strlcpy(lang, "LANG=C", sizeof(lang));

  strlcpy(remote_addr, "REMOTE_ADDR=", sizeof(remote_addr));
  httpAddrString(httpGetAddress(con->http), remote_addr + 12,
                 sizeof(remote_addr) - 12);

  snprintf(remote_host, sizeof(remote_host), "REMOTE_HOST=%s",
           httpGetHostname(con->http, NULL, 0));

  snprintf(script_name, sizeof(script_name), "SCRIPT_NAME=%s", con->uri);
  if ((uriptr = strchr(script_name, '?')) != NULL)
    *uriptr = '\0';

  snprintf(script_filename, sizeof(script_filename), "SCRIPT_FILENAME=%s%s",
           DocumentRoot, script_name + 12);

  snprintf(server_port, sizeof(server_port), "SERVER_PORT=%d", con->serverport);

  if (httpGetField(con->http, HTTP_FIELD_HOST)[0])
  {
    char *nameptr;			 

    snprintf(server_name, sizeof(server_name), "SERVER_NAME=%s",
	     httpGetField(con->http, HTTP_FIELD_HOST));
    if ((nameptr = strrchr(server_name, ':')) != NULL && !strchr(nameptr, ']'))
      *nameptr = '\0';			 
  }
  else
    snprintf(server_name, sizeof(server_name), "SERVER_NAME=%s",
	     con->servername);

  envc = cupsdLoadEnv(envp, (int)(sizeof(envp) / sizeof(envp[0])));

  if (auth_type[0])
    envp[envc ++] = auth_type;

  envp[envc ++] = lang;
  envp[envc ++] = "REDIRECT_STATUS=1";
  envp[envc ++] = "GATEWAY_INTERFACE=CGI/1.1";
  envp[envc ++] = server_name;
  envp[envc ++] = server_port;
  envp[envc ++] = remote_addr;
  envp[envc ++] = remote_host;
  envp[envc ++] = script_name;
  envp[envc ++] = script_filename;

  if (path_info[0])
    envp[envc ++] = path_info;

  if (con->username[0])
  {
    snprintf(remote_user, sizeof(remote_user), "REMOTE_USER=%s", con->username);

    envp[envc ++] = remote_user;
  }

  if (httpGetVersion(con->http) == HTTP_VERSION_1_1)
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/1.1";
  else if (httpGetVersion(con->http) == HTTP_VERSION_1_0)
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/1.0";
  else
    envp[envc ++] = "SERVER_PROTOCOL=HTTP/0.9";

  if (httpGetCookie(con->http))
  {
    snprintf(http_cookie, sizeof(http_cookie), "HTTP_COOKIE=%s",
             httpGetCookie(con->http));
    envp[envc ++] = http_cookie;
  }

  if (httpGetField(con->http, HTTP_FIELD_USER_AGENT)[0])
  {
    snprintf(http_user_agent, sizeof(http_user_agent), "HTTP_USER_AGENT=%s",
             httpGetField(con->http, HTTP_FIELD_USER_AGENT));
    envp[envc ++] = http_user_agent;
  }

  if (httpGetField(con->http, HTTP_FIELD_REFERER)[0])
  {
    snprintf(http_referer, sizeof(http_referer), "HTTP_REFERER=%s",
             httpGetField(con->http, HTTP_FIELD_REFERER));
    envp[envc ++] = http_referer;
  }

  if (con->operation == HTTP_STATE_GET)
  {
    envp[envc ++] = "REQUEST_METHOD=GET";

    if (con->query_string)
    {
      

      envp[envc ++] = con->query_string;
    }
    else
      envp[envc ++] = "QUERY_STRING=";
  }
  else
  {
    sprintf(content_length, "CONTENT_LENGTH=" CUPS_LLFMT,
            CUPS_LLCAST con->bytes);
    snprintf(content_type, sizeof(content_type), "CONTENT_TYPE=%s",
             httpGetField(con->http, HTTP_FIELD_CONTENT_TYPE));

    envp[envc ++] = "REQUEST_METHOD=POST";
    envp[envc ++] = content_length;
    envp[envc ++] = content_type;
  }

  

  if (httpIsEncrypted(con->http))
    envp[envc ++] = "HTTPS=ON";

  

  envp[envc] = NULL;

  if (LogLevel >= CUPSD_LOG_DEBUG)
  {
    for (i = 0; i < argc; i ++)
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "[CGI] argv[%d] = \"%s\"", i, argv[i]);
    for (i = 0; i < envc; i ++)
      cupsdLogMessage(CUPSD_LOG_DEBUG,
                      "[CGI] envp[%d] = \"%s\"", i, envp[i]);
  }

  

  if (cupsdOpenPipe(fds))
  {
    cupsdLogMessage(CUPSD_LOG_ERROR, "[CGI] Unable to create pipe for %s - %s",
                    argv[0], strerror(errno));
    return (0);
  }

  

  if (cupsdStartProcess(command, argv, envp, infile, fds[1], CGIPipes[1],
			-1, -1, root, DefaultProfile, NULL, &pid) < 0)
  {
    

    cupsdLogMessage(CUPSD_LOG_ERROR, "[CGI] Unable to start %s - %s", argv[0],
                    strerror(errno));

    cupsdClosePipe(fds);
    pid = 0;
  }
  else
  {
    

    if (con->username[0])
      cupsdAddCert(pid, con->username, con->type);

    cupsdLogMessage(CUPSD_LOG_DEBUG, "[CGI] Started %s (PID %d)", command, pid);

    *outfile = fds[0];
    close(fds[1]);
  }

  return (pid);
}
