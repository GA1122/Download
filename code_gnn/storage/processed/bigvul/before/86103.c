cupsdSendHeader(
    cupsd_client_t *con,		 
    http_status_t  code,		 
    char           *type,		 
    int            auth_type)		 
{
  char		auth_str[1024];		 


  cupsdLogClient(con, CUPSD_LOG_DEBUG, "cupsdSendHeader: code=%d, type=\"%s\", auth_type=%d", code, type, auth_type);

  

  if (code == HTTP_STATUS_CUPS_WEBIF_DISABLED)
  {
    

    code = HTTP_STATUS_OK;
  }

  if (ServerHeader)
    httpSetField(con->http, HTTP_FIELD_SERVER, ServerHeader);

  if (code == HTTP_STATUS_METHOD_NOT_ALLOWED)
    httpSetField(con->http, HTTP_FIELD_ALLOW, "GET, HEAD, OPTIONS, POST, PUT");

  if (code == HTTP_STATUS_UNAUTHORIZED)
  {
    if (auth_type == CUPSD_AUTH_NONE)
    {
      if (!con->best || con->best->type <= CUPSD_AUTH_NONE)
	auth_type = cupsdDefaultAuthType();
      else
	auth_type = con->best->type;
    }

    auth_str[0] = '\0';

    if (auth_type == CUPSD_AUTH_BASIC)
      strlcpy(auth_str, "Basic realm=\"CUPS\"", sizeof(auth_str));
#ifdef HAVE_GSSAPI
    else if (auth_type == CUPSD_AUTH_NEGOTIATE)
    {
#  ifdef AF_LOCAL
      if (httpAddrFamily(httpGetAddress(con->http)) == AF_LOCAL)
        strlcpy(auth_str, "Basic realm=\"CUPS\"", sizeof(auth_str));
      else
#  endif  
      strlcpy(auth_str, "Negotiate", sizeof(auth_str));
    }
#endif  

    if (con->best && auth_type != CUPSD_AUTH_NEGOTIATE &&
        !_cups_strcasecmp(httpGetHostname(con->http, NULL, 0), "localhost"))
    {
      

      char	*name,			 
		*auth_key;		 
      size_t	auth_size;		 

      auth_key  = auth_str + strlen(auth_str);
      auth_size = sizeof(auth_str) - (size_t)(auth_key - auth_str);

      for (name = (char *)cupsArrayFirst(con->best->names);
           name;
	   name = (char *)cupsArrayNext(con->best->names))
      {
#ifdef HAVE_AUTHORIZATION_H
	if (!_cups_strncasecmp(name, "@AUTHKEY(", 9))
	{
	  snprintf(auth_key, auth_size, ", authkey=\"%s\"", name + 9);
	   
	  break;
        }
	else
#endif  
	if (!_cups_strcasecmp(name, "@SYSTEM"))
	{
#ifdef HAVE_AUTHORIZATION_H
	  if (SystemGroupAuthKey)
	    snprintf(auth_key, auth_size,
	             ", authkey=\"%s\"",
		     SystemGroupAuthKey);
          else
#else
	  strlcpy(auth_key, ", trc=\"y\"", auth_size);
#endif  
	  break;
	}
      }
    }

    if (auth_str[0])
    {
      cupsdLogClient(con, CUPSD_LOG_DEBUG, "WWW-Authenticate: %s", auth_str);

      httpSetField(con->http, HTTP_FIELD_WWW_AUTHENTICATE, auth_str);
    }
  }

  if (con->language && strcmp(con->language->language, "C"))
    httpSetField(con->http, HTTP_FIELD_CONTENT_LANGUAGE, con->language->language);

  if (type)
  {
    if (!strcmp(type, "text/html"))
      httpSetField(con->http, HTTP_FIELD_CONTENT_TYPE, "text/html; charset=utf-8");
    else
      httpSetField(con->http, HTTP_FIELD_CONTENT_TYPE, type);
  }

  return (!httpWriteResponse(con->http, code));
}
