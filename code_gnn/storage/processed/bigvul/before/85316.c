cancel_all_jobs(cupsd_client_t  *con,	 
	        ipp_attribute_t *uri)	 
{
  int		i;			 
  http_status_t	status;			 
  cups_ptype_t	dtype;			 
  char		scheme[HTTP_MAX_URI],	 
		userpass[HTTP_MAX_URI],	 
		hostname[HTTP_MAX_URI],	 
		resource[HTTP_MAX_URI];	 
  int		port;			 
  ipp_attribute_t *attr;		 
  const char	*username = NULL;	 
  cupsd_jobaction_t purge = CUPSD_JOB_DEFAULT;
					 
  cupsd_printer_t *printer;		 
  ipp_attribute_t *job_ids;		 
  cupsd_job_t	*job;			 


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "cancel_all_jobs(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

  

  switch (con->request->request.op.operation_id)
  {
    case IPP_PURGE_JOBS :
        

        if ((attr = ippFindAttribute(con->request, "my-jobs",
                                     IPP_TAG_BOOLEAN)) != NULL &&
            attr->values[0].boolean)
	{
	  if ((attr = ippFindAttribute(con->request, "requesting-user-name",
				       IPP_TAG_NAME)) != NULL)
	    username = attr->values[0].string.text;
	  else
	  {
	    send_ipp_status(con, IPP_BAD_REQUEST,
			    _("Missing requesting-user-name attribute."));
	    return;
	  }
	}

        

	if ((attr = ippFindAttribute(con->request, "purge-jobs",
				     IPP_TAG_BOOLEAN)) != NULL)
	  purge = attr->values[0].boolean ? CUPSD_JOB_PURGE : CUPSD_JOB_DEFAULT;
	else
	  purge = CUPSD_JOB_PURGE;
	break;

    case IPP_CANCEL_MY_JOBS :
        if (con->username[0])
          username = con->username;
        else if ((attr = ippFindAttribute(con->request, "requesting-user-name",
					  IPP_TAG_NAME)) != NULL)
          username = attr->values[0].string.text;
        else
        {
	  send_ipp_status(con, IPP_BAD_REQUEST,
			  _("Missing requesting-user-name attribute."));
	  return;
        }

    default :
        break;
  }

  job_ids = ippFindAttribute(con->request, "job-ids", IPP_TAG_INTEGER);

  

  if (strcmp(uri->name, "printer-uri"))
  {
    send_ipp_status(con, IPP_BAD_REQUEST,
                    _("The printer-uri attribute is required."));
    return;
  }

  

  if (!cupsdValidateDest(uri->values[0].string.text, &dtype, &printer))
  {
    

    httpSeparateURI(HTTP_URI_CODING_ALL, uri->values[0].string.text,
                    scheme, sizeof(scheme), userpass, sizeof(userpass),
		    hostname, sizeof(hostname), &port,
		    resource, sizeof(resource));

    if ((!strncmp(resource, "/printers/", 10) && resource[10]) ||
        (!strncmp(resource, "/classes/", 9) && resource[9]))
    {
      send_ipp_status(con, IPP_NOT_FOUND,
                      _("The printer or class does not exist."));
      return;
    }

    

    if ((status = cupsdCheckPolicy(DefaultPolicyPtr, con, NULL)) != HTTP_OK)
    {
      send_http_error(con, status, NULL);
      return;
    }

    if (job_ids)
    {
      for (i = 0; i < job_ids->num_values; i ++)
      {
	if ((job = cupsdFindJob(job_ids->values[i].integer)) == NULL)
	  break;

        if (con->request->request.op.operation_id == IPP_CANCEL_MY_JOBS &&
            _cups_strcasecmp(job->username, username))
          break;
      }

      if (i < job_ids->num_values)
      {
	send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."),
			job_ids->values[i].integer);
	return;
      }

      for (i = 0; i < job_ids->num_values; i ++)
      {
	job = cupsdFindJob(job_ids->values[i].integer);

	cupsdSetJobState(job, IPP_JOB_CANCELED, purge,
	                 purge == CUPSD_JOB_PURGE ? "Job purged by user." :
	                                            "Job canceled by user.");
      }

      cupsdLogMessage(CUPSD_LOG_INFO, "Selected jobs were %s by \"%s\".",
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
    else
    {
      

      cupsdCancelJobs(NULL, username, purge);

      cupsdLogMessage(CUPSD_LOG_INFO, "All jobs were %s by \"%s\".",
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
  }
  else
  {
    

    if ((status = cupsdCheckPolicy(printer->op_policy_ptr, con,
                                   NULL)) != HTTP_OK)
    {
      send_http_error(con, status, printer);
      return;
    }

    if (job_ids)
    {
      for (i = 0; i < job_ids->num_values; i ++)
      {
	if ((job = cupsdFindJob(job_ids->values[i].integer)) == NULL ||
	    _cups_strcasecmp(job->dest, printer->name))
	  break;

        if (con->request->request.op.operation_id == IPP_CANCEL_MY_JOBS &&
            _cups_strcasecmp(job->username, username))
          break;
      }

      if (i < job_ids->num_values)
      {
	send_ipp_status(con, IPP_NOT_FOUND, _("Job #%d does not exist."),
			job_ids->values[i].integer);
	return;
      }

      for (i = 0; i < job_ids->num_values; i ++)
      {
	job = cupsdFindJob(job_ids->values[i].integer);

	cupsdSetJobState(job, IPP_JOB_CANCELED, purge,
	                 purge == CUPSD_JOB_PURGE ? "Job purged by user." :
	                                            "Job canceled by user.");
      }

      cupsdLogMessage(CUPSD_LOG_INFO, "Selected jobs were %s by \"%s\".",
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
    else
    {
      

      cupsdCancelJobs(printer->name, username, purge);

      cupsdLogMessage(CUPSD_LOG_INFO, "All jobs on \"%s\" were %s by \"%s\".",
		      printer->name,
		      purge == CUPSD_JOB_PURGE ? "purged" : "canceled",
		      get_username(con));
    }
  }

  con->response->request.status.status_code = IPP_OK;

  cupsdCheckJobs();
}
