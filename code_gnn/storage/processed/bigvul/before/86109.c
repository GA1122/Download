is_cgi(cupsd_client_t *con,		 
       const char     *filename,	 
       struct stat    *filestats,	 
       mime_type_t    *type)		 
{
  const char	*options;		 


  

  if ((options = strchr(con->uri, '?')) != NULL)
  {
    options ++;
    cupsdSetStringf(&(con->query_string), "QUERY_STRING=%s", options);
  }

  

  if (!type || _cups_strcasecmp(type->super, "application"))
  {
    cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 0.", filename, filestats, type ? type->super : "unknown", type ? type->type : "unknown");
    return (0);
  }

  if (!_cups_strcasecmp(type->type, "x-httpd-cgi") &&
      (filestats->st_mode & 0111))
  {
    

    cupsdSetString(&con->command, filename);

    if (options)
      cupsdSetStringf(&con->options, " %s", options);

    cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 1.", filename, filestats, type->super, type->type);
    return (1);
  }
#ifdef HAVE_JAVA
  else if (!_cups_strcasecmp(type->type, "x-httpd-java"))
  {
    

    cupsdSetString(&con->command, CUPS_JAVA);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 1.", filename, filestats, type->super, type->type);
    return (1);
  }
#endif  
#ifdef HAVE_PERL
  else if (!_cups_strcasecmp(type->type, "x-httpd-perl"))
  {
    

    cupsdSetString(&con->command, CUPS_PERL);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 1.", filename, filestats, type->super, type->type);
    return (1);
  }
#endif  
#ifdef HAVE_PHP
  else if (!_cups_strcasecmp(type->type, "x-httpd-php"))
  {
    

    cupsdSetString(&con->command, CUPS_PHP);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 1.", filename, filestats, type->super, type->type);
    return (1);
  }
#endif  
#ifdef HAVE_PYTHON
  else if (!_cups_strcasecmp(type->type, "x-httpd-python"))
  {
    

    cupsdSetString(&con->command, CUPS_PYTHON);

    if (options)
      cupsdSetStringf(&con->options, " %s %s", filename, options);
    else
      cupsdSetStringf(&con->options, " %s", filename);

    cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 1.", filename, filestats, type->super, type->type);
    return (1);
  }
#endif  

  cupsdLogClient(con, CUPSD_LOG_DEBUG2, "is_cgi: filename=\"%s\", filestats=%p, type=%s/%s, returning 0.", filename, filestats, type->super, type->type);
  return (0);
}
