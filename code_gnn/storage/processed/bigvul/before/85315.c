authenticate_job(cupsd_client_t  *con,	 
	         ipp_attribute_t *uri)	 
{
  ipp_attribute_t	*attr,		 
			*auth_info;	 
  int			jobid;		 
  cupsd_job_t		*job;		 
  char			scheme[HTTP_MAX_URI],
					 
			username[HTTP_MAX_URI],
					 
			host[HTTP_MAX_URI],
					 
			resource[HTTP_MAX_URI];
					 
  int			port;		 


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "authenticate_job(%p[%d], %s)",
                  con, con->number, uri->values[0].string.text);

  

  con->response->request.status.status_code = IPP_OK;

  

  if (!strcmp(uri->name, "printer-uri"))
  {
    

    if ((attr = ippFindAttribute(con->request, "job-id",
                                 IPP_TAG_INTEGER)) == NULL)
    {
      send_ipp_status(con, IPP_BAD_REQUEST,
                      _("Got a printer-uri attribute but no job-id."));
      return;
    }

    jobid = attr->values[0].integer;
  }
  else
  {
    

    httpSeparateURI(HTTP_URI_CODING_ALL, uri->values[0].string.text, scheme,
                    sizeof(scheme), username, sizeof(username), host,
		    sizeof(host), &port, resource, sizeof(resource));

    if (strncmp(resource, "/jobs/", 6))
    {
      

      send_ipp_status(con, IPP_BAD_REQUEST, _("Bad job-uri \"%s\"."),
                      uri->values[0].string.text);
      return;
    }

    jobid = atoi(resource + 6);
  }

  

  if ((job = cupsdFindJob(jobid)) == NULL)
  {
    

    send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."), jobid);
    return;
  }

  

  if (job->state_value != IPP_JOB_HELD)
  {
    

    send_ipp_status(con, IPP_NOT_POSSIBLE,
                    _("Job #%d is not held for authentication."),
		    jobid);
    return;
  }

  

  auth_info = ippFindAttribute(con->request, "auth-info", IPP_TAG_TEXT);

  if (!con->username[0] && !auth_info)
  {
    cupsd_printer_t	*printer;	 

    

    printer = cupsdFindDest(job->dest);

    if (printer && printer->num_auth_info_required > 0 &&
        !strcmp(printer->auth_info_required[0], "negotiate"))
      send_http_error(con, HTTP_UNAUTHORIZED, printer);
    else
      send_ipp_status(con, IPP_NOT_AUTHORIZED,
		      _("No authentication information provided."));
    return;
  }

  

  if (!validate_user(job, con, job->username, username, sizeof(username)))
  {
    send_http_error(con, con->username[0] ? HTTP_FORBIDDEN : HTTP_UNAUTHORIZED,
                    cupsdFindDest(job->dest));
    return;
  }

  

  save_auth_info(con, job, auth_info);

  

  if ((attr = ippFindAttribute(job->attrs, "job-hold-until",
                               IPP_TAG_KEYWORD)) == NULL)
    attr = ippFindAttribute(job->attrs, "job-hold-until", IPP_TAG_NAME);

  if (attr)
  {
    ippSetValueTag(job->attrs, &attr, IPP_TAG_KEYWORD);
    ippSetString(job->attrs, &attr, 0, "no-hold");
  }

  

  cupsdReleaseJob(job);

  cupsdAddEvent(CUPSD_EVENT_JOB_STATE, NULL, job, "Job authenticated by user");

  cupsdLogJob(job, CUPSD_LOG_INFO, "Authenticated by \"%s\".", con->username);

  cupsdCheckJobs();
}
