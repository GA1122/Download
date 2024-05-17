 add_job(cupsd_client_t  *con,		 
 	cupsd_printer_t *printer,	 
 	mime_type_t     *filetype)	 
 {
   http_status_t	status;			 
   ipp_attribute_t *attr,		 
 		*auth_info;		 
   const char	*mandatory;		 
   const char	*val;			 
   int		priority;		 
   cupsd_job_t	*job;			 
   char		job_uri[HTTP_MAX_URI];	 
   int		kbytes;			 
   int		i;			 
   int		lowerpagerange;		 
   int		exact;			 
   ipp_attribute_t *media_col,		 
 		*media_margin;		 
   ipp_t		*unsup_col;		 
   static const char * const readonly[] = 
   {
     "date-time-at-completed",
     "date-time-at-creation",
     "date-time-at-processing",
     "job-detailed-status-messages",
     "job-document-access-errors",
     "job-id",
     "job-impressions-completed",
     "job-k-octets-completed",
     "job-media-sheets-completed",
     "job-pages-completed",
     "job-printer-up-time",
     "job-printer-uri",
     "job-state",
     "job-state-message",
     "job-state-reasons",
     "job-uri",
     "number-of-documents",
     "number-of-intervening-jobs",
     "output-device-assigned",
     "time-at-completed",
     "time-at-creation",
     "time-at-processing"
   };
 
 
   cupsdLogMessage(CUPSD_LOG_DEBUG2, "add_job(%p[%d], %p(%s), %p(%s/%s))",
                   con, con->number, printer, printer->name,
 		  filetype, filetype ? filetype->super : "none",
 		  filetype ? filetype->type : "none");
 
   
 
   if (!printer->shared &&
       _cups_strcasecmp(con->http->hostname, "localhost") &&
       _cups_strcasecmp(con->http->hostname, ServerName))
   {
     send_ipp_status(con, IPP_NOT_AUTHORIZED,
                     _("The printer or class is not shared."));
     return (NULL);
   }
 
   
 
   auth_info = ippFindAttribute(con->request, "auth-info", IPP_TAG_TEXT);
 
   if ((status = cupsdCheckPolicy(printer->op_policy_ptr, con, NULL)) != HTTP_OK)
   {
     send_http_error(con, status, printer);
     return (NULL);
   }
   else if (printer->num_auth_info_required == 1 &&
            !strcmp(printer->auth_info_required[0], "negotiate") &&
            !con->username[0])
   {
     send_http_error(con, HTTP_UNAUTHORIZED, printer);
     return (NULL);
   }
 #ifdef HAVE_SSL
   else if (auth_info && !con->http->tls &&
            !httpAddrLocalhost(con->http->hostaddr))
   {
     
 
     send_http_error(con, HTTP_UPGRADE_REQUIRED, printer);
     return (NULL);
   }
 #endif  
 
   
 
   if (!printer->accepting)
   {
     send_ipp_status(con, IPP_NOT_ACCEPTING,
                     _("Destination \"%s\" is not accepting jobs."),
                     printer->name);
     return (NULL);
   }
 
   
 
   for (i = 0; i < (int)(sizeof(readonly) / sizeof(readonly[0])); i ++)
   {
     if ((attr = ippFindAttribute(con->request, readonly[i], IPP_TAG_ZERO)) != NULL)
     {
       ippDeleteAttribute(con->request, attr);
 
       if (StrictConformance)
       {
 	send_ipp_status(con, IPP_BAD_REQUEST, _("The '%s' Job Status attribute cannot be supplied in a job creation request."), readonly[i]);
 	return (NULL);
       }
 
       cupsdLogMessage(CUPSD_LOG_INFO, "Unexpected '%s' Job Status attribute in a job creation request.", readonly[i]);
     }
   }
 
   if (printer->pc)
   {
     for (mandatory = (char *)cupsArrayFirst(printer->pc->mandatory);
 	 mandatory;
 	 mandatory = (char *)cupsArrayNext(printer->pc->mandatory))
     {
       if (!ippFindAttribute(con->request, mandatory, IPP_TAG_ZERO))
       {
         
 
 	send_ipp_status(con, IPP_CONFLICT,
 			_("The \"%s\" attribute is required for print jobs."),
 			mandatory);
 	return (NULL);
       }
     }
   }
 
   if (filetype && printer->filetypes &&
       !cupsArrayFind(printer->filetypes, filetype))
   {
     char	mimetype[MIME_MAX_SUPER + MIME_MAX_TYPE + 2];
 					 
 
 
     snprintf(mimetype, sizeof(mimetype), "%s/%s", filetype->super,
              filetype->type);
 
     send_ipp_status(con, IPP_DOCUMENT_FORMAT,
                     _("Unsupported format \"%s\"."), mimetype);
 
     ippAddString(con->response, IPP_TAG_UNSUPPORTED_GROUP, IPP_TAG_MIMETYPE,
                  "document-format", NULL, mimetype);
 
     return (NULL);
   }
 
   if ((attr = ippFindAttribute(con->request, "copies",
                                IPP_TAG_INTEGER)) != NULL)
   {
     if (attr->values[0].integer < 1 || attr->values[0].integer > MaxCopies)
     {
       send_ipp_status(con, IPP_ATTRIBUTES, _("Bad copies value %d."),
                       attr->values[0].integer);
       ippAddInteger(con->response, IPP_TAG_UNSUPPORTED_GROUP, IPP_TAG_INTEGER,
 	            "copies", attr->values[0].integer);
       return (NULL);
     }
   }
 
   if ((attr = ippFindAttribute(con->request, "job-sheets",
                                IPP_TAG_ZERO)) != NULL)
   {
     if (attr->value_tag != IPP_TAG_KEYWORD &&
         attr->value_tag != IPP_TAG_NAME)
     {
       send_ipp_status(con, IPP_BAD_REQUEST, _("Bad job-sheets value type."));
       return (NULL);
     }
 
     if (attr->num_values > 2)
     {
       send_ipp_status(con, IPP_BAD_REQUEST,
                       _("Too many job-sheets values (%d > 2)."),
 		      attr->num_values);
       return (NULL);
     }
 
     for (i = 0; i < attr->num_values; i ++)
       if (strcmp(attr->values[i].string.text, "none") &&
           !cupsdFindBanner(attr->values[i].string.text))
       {
 	send_ipp_status(con, IPP_BAD_REQUEST, _("Bad job-sheets value \"%s\"."),
 			attr->values[i].string.text);
 	return (NULL);
       }
   }
 
   if ((attr = ippFindAttribute(con->request, "number-up",
                                IPP_TAG_INTEGER)) != NULL)
   {
     if (attr->values[0].integer != 1 &&
         attr->values[0].integer != 2 &&
         attr->values[0].integer != 4 &&
         attr->values[0].integer != 6 &&
         attr->values[0].integer != 9 &&
         attr->values[0].integer != 16)
     {
       send_ipp_status(con, IPP_ATTRIBUTES, _("Bad number-up value %d."),
                       attr->values[0].integer);
       ippAddInteger(con->response, IPP_TAG_UNSUPPORTED_GROUP, IPP_TAG_INTEGER,
 	            "number-up", attr->values[0].integer);
       return (NULL);
     }
   }
 
   if ((attr = ippFindAttribute(con->request, "page-ranges",
                                IPP_TAG_RANGE)) != NULL)
   {
     for (i = 0, lowerpagerange = 1; i < attr->num_values; i ++)
     {
       if (attr->values[i].range.lower < lowerpagerange ||
 	  attr->values[i].range.lower > attr->values[i].range.upper)
       {
 	send_ipp_status(con, IPP_BAD_REQUEST,
 	                _("Bad page-ranges values %d-%d."),
 	                attr->values[i].range.lower,
 			attr->values[i].range.upper);
 	return (NULL);
       }
 
       lowerpagerange = attr->values[i].range.upper + 1;
     }
   }
 
   
 
   if (!ippFindAttribute(con->request, "PageRegion", IPP_TAG_ZERO) &&
       !ippFindAttribute(con->request, "PageSize", IPP_TAG_ZERO) &&
       _ppdCacheGetPageSize(printer->pc, con->request, NULL, &exact))
   {
     if (!exact &&
         (media_col = ippFindAttribute(con->request, "media-col",
 	                              IPP_TAG_BEGIN_COLLECTION)) != NULL)
     {
       send_ipp_status(con, IPP_OK_SUBST, _("Unsupported margins."));
 
       unsup_col = ippNew();
       if ((media_margin = ippFindAttribute(media_col->values[0].collection,
                                            "media-bottom-margin",
 					   IPP_TAG_INTEGER)) != NULL)
         ippAddInteger(unsup_col, IPP_TAG_ZERO, IPP_TAG_INTEGER,
 	              "media-bottom-margin", media_margin->values[0].integer);
 
       if ((media_margin = ippFindAttribute(media_col->values[0].collection,
                                            "media-left-margin",
 					   IPP_TAG_INTEGER)) != NULL)
         ippAddInteger(unsup_col, IPP_TAG_ZERO, IPP_TAG_INTEGER,
 	              "media-left-margin", media_margin->values[0].integer);
 
       if ((media_margin = ippFindAttribute(media_col->values[0].collection,
                                            "media-right-margin",
 					   IPP_TAG_INTEGER)) != NULL)
         ippAddInteger(unsup_col, IPP_TAG_ZERO, IPP_TAG_INTEGER,
 	              "media-right-margin", media_margin->values[0].integer);
 
       if ((media_margin = ippFindAttribute(media_col->values[0].collection,
                                            "media-top-margin",
 					   IPP_TAG_INTEGER)) != NULL)
         ippAddInteger(unsup_col, IPP_TAG_ZERO, IPP_TAG_INTEGER,
 	              "media-top-margin", media_margin->values[0].integer);
 
       ippAddCollection(con->response, IPP_TAG_UNSUPPORTED_GROUP, "media-col",
                        unsup_col);
       ippDelete(unsup_col);
     }
   }
 
   
 
   if (MaxJobs && cupsArrayCount(Jobs) >= MaxJobs)
     cupsdCleanJobs();
 
   if (MaxJobs && cupsArrayCount(Jobs) >= MaxJobs)
   {
     send_ipp_status(con, IPP_NOT_POSSIBLE, _("Too many active jobs."));
     return (NULL);
   }
 
   if ((i = check_quotas(con, printer)) < 0)
   {
     send_ipp_status(con, IPP_NOT_POSSIBLE, _("Quota limit reached."));
     return (NULL);
   }
   else if (i == 0)
   {
     send_ipp_status(con, IPP_NOT_AUTHORIZED, _("Not allowed to print."));
     return (NULL);
   }
 
   
 
   if ((attr = ippFindAttribute(con->request, "job-priority",
                                IPP_TAG_INTEGER)) != NULL)
     priority = attr->values[0].integer;
   else
   {
     if ((val = cupsGetOption("job-priority", printer->num_options,
                              printer->options)) != NULL)
       priority = atoi(val);
     else
       priority = 50;
 
     ippAddInteger(con->request, IPP_TAG_JOB, IPP_TAG_INTEGER, "job-priority",
                   priority);
   }
 
   if ((attr = ippFindAttribute(con->request, "job-name", IPP_TAG_ZERO)) == NULL)
     ippAddString(con->request, IPP_TAG_JOB, IPP_TAG_NAME, "job-name", NULL, "Untitled");
   else if ((attr->value_tag != IPP_TAG_NAME &&
             attr->value_tag != IPP_TAG_NAMELANG) ||
            attr->num_values != 1)
   {
     send_ipp_status(con, IPP_ATTRIBUTES,
                     _("Bad job-name value: Wrong type or count."));
     if ((attr = ippCopyAttribute(con->response, attr, 0)) != NULL)
       attr->group_tag = IPP_TAG_UNSUPPORTED_GROUP;
     return (NULL);
   }
   else if (!ippValidateAttribute(attr))
   {
     send_ipp_status(con, IPP_ATTRIBUTES, _("Bad job-name value: %s"),
                     cupsLastErrorString());
     if ((attr = ippCopyAttribute(con->response, attr, 0)) != NULL)
       attr->group_tag = IPP_TAG_UNSUPPORTED_GROUP;
      return (NULL);
    }
  
//   attr = ippFindAttribute(con->request, "requesting-user-name", IPP_TAG_NAME);
// 
//   if (attr && !ippValidateAttribute(attr))
//   {
//     send_ipp_status(con, IPP_ATTRIBUTES, _("Bad requesting-user-name value: %s"), cupsLastErrorString());
//     if ((attr = ippCopyAttribute(con->response, attr, 0)) != NULL)
//       attr->group_tag = IPP_TAG_UNSUPPORTED_GROUP;
//     return (NULL);
//   }
// 
    if ((job = cupsdAddJob(priority, printer->name)) == NULL)
    {
      send_ipp_status(con, IPP_INTERNAL_ERROR,
                     _("Unable to add job for destination \"%s\"."),
 		    printer->name);
     return (NULL);
   }
 
   job->dtype   = printer->type & (CUPS_PRINTER_CLASS | CUPS_PRINTER_REMOTE);
   job->attrs   = con->request;
   job->dirty   = 1;
   con->request = ippNewRequest(job->attrs->request.op.operation_id);
 
   cupsdMarkDirty(CUPSD_DIRTY_JOBS);
 
    add_job_uuid(job);
    apply_printer_defaults(printer, job);
  
  attr = ippFindAttribute(job->attrs, "requesting-user-name", IPP_TAG_NAME);
    if (con->username[0])
    {
      cupsdSetString(&job->username, con->username);
 
     if (attr)
       ippSetString(job->attrs, &attr, 0, con->username);
   }
   else if (attr)
   {
     cupsdLogMessage(CUPSD_LOG_DEBUG,
                     "add_job: requesting-user-name=\"%s\"",
                     attr->values[0].string.text);
 
     cupsdSetString(&job->username, attr->values[0].string.text);
   }
   else
     cupsdSetString(&job->username, "anonymous");
 
   if (!attr)
     ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_NAME,
                  "job-originating-user-name", NULL, job->username);
   else
   {
     ippSetGroupTag(job->attrs, &attr, IPP_TAG_JOB);
     ippSetName(job->attrs, &attr, "job-originating-user-name");
   }
 
   if (con->username[0] || auth_info)
   {
     save_auth_info(con, job, auth_info);
 
     
 
     if (auth_info)
       ippDeleteAttribute(job->attrs, auth_info);
   }
 
   if ((attr = ippFindAttribute(con->request, "job-name", IPP_TAG_NAME)) != NULL)
     cupsdSetString(&(job->name), attr->values[0].string.text);
 
   if ((attr = ippFindAttribute(job->attrs, "job-originating-host-name",
                                IPP_TAG_ZERO)) != NULL)
   {
     
 
     if (attr->value_tag != IPP_TAG_NAME ||
         attr->num_values != 1 ||
         strcmp(con->http->hostname, "localhost"))
     {
       
 
       ippDeleteAttribute(job->attrs, attr);
       ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_NAME, "job-originating-host-name", NULL, con->http->hostname);
     }
     else
       ippSetGroupTag(job->attrs, &attr, IPP_TAG_JOB);
   }
   else
   {
     
 
     ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_NAME,
         	 "job-originating-host-name", NULL, con->http->hostname);
   }
 
   ippAddOutOfBand(job->attrs, IPP_TAG_JOB, IPP_TAG_NOVALUE, "date-time-at-completed");
   ippAddDate(job->attrs, IPP_TAG_JOB, "date-time-at-creation", ippTimeToDate(time(NULL)));
   ippAddOutOfBand(job->attrs, IPP_TAG_JOB, IPP_TAG_NOVALUE, "date-time-at-processing");
   ippAddOutOfBand(job->attrs, IPP_TAG_JOB, IPP_TAG_NOVALUE, "time-at-completed");
   ippAddInteger(job->attrs, IPP_TAG_JOB, IPP_TAG_INTEGER, "time-at-creation", time(NULL));
   ippAddOutOfBand(job->attrs, IPP_TAG_JOB, IPP_TAG_NOVALUE, "time-at-processing");
 
   
 
   ippAddInteger(job->attrs, IPP_TAG_JOB, IPP_TAG_INTEGER, "job-id", job->id);
   job->state = ippAddInteger(job->attrs, IPP_TAG_JOB, IPP_TAG_ENUM,
                              "job-state", IPP_JOB_STOPPED);
   job->state_value = (ipp_jstate_t)job->state->values[0].integer;
   job->reasons = ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_KEYWORD,
                               "job-state-reasons", NULL, "job-incoming");
   job->impressions = ippAddInteger(job->attrs, IPP_TAG_JOB, IPP_TAG_INTEGER, "job-impressions-completed", 0);
   job->sheets = ippAddInteger(job->attrs, IPP_TAG_JOB, IPP_TAG_INTEGER,
                               "job-media-sheets-completed", 0);
   ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_URI, "job-printer-uri", NULL,
                printer->uri);
 
   if ((attr = ippFindAttribute(job->attrs, "job-k-octets", IPP_TAG_INTEGER)) != NULL)
     attr->values[0].integer = 0;
   else
     ippAddInteger(job->attrs, IPP_TAG_JOB, IPP_TAG_INTEGER, "job-k-octets", 0);
 
   if ((attr = ippFindAttribute(job->attrs, "job-hold-until",
                                IPP_TAG_KEYWORD)) == NULL)
     attr = ippFindAttribute(job->attrs, "job-hold-until", IPP_TAG_NAME);
   if (!attr)
   {
     if ((val = cupsGetOption("job-hold-until", printer->num_options,
                              printer->options)) == NULL)
       val = "no-hold";
 
     attr = ippAddString(job->attrs, IPP_TAG_JOB, IPP_TAG_KEYWORD,
                         "job-hold-until", NULL, val);
   }
 
   if (printer->holding_new_jobs)
   {
     
 
     if (attr && strcmp(attr->values[0].string.text, "no-hold"))
       cupsdSetJobHoldUntil(job, ippGetString(attr, 0, NULL), 0);
     else
       cupsdSetJobHoldUntil(job, "indefinite", 0);
 
     job->state->values[0].integer = IPP_JOB_HELD;
     job->state_value              = IPP_JOB_HELD;
 
     ippSetString(job->attrs, &job->reasons, 0, "job-held-on-create");
   }
   else if (attr && strcmp(attr->values[0].string.text, "no-hold"))
   {
     
 
     cupsdSetJobHoldUntil(job, attr->values[0].string.text, 0);
 
     job->state->values[0].integer = IPP_JOB_HELD;
     job->state_value              = IPP_JOB_HELD;
 
     ippSetString(job->attrs, &job->reasons, 0, "job-hold-until-specified");
   }
   else if (job->attrs->request.op.operation_id == IPP_CREATE_JOB)
   {
     job->hold_until               = time(NULL) + MultipleOperationTimeout;
     job->state->values[0].integer = IPP_JOB_HELD;
     job->state_value              = IPP_JOB_HELD;
   }
   else
   {
     job->state->values[0].integer = IPP_JOB_PENDING;
     job->state_value              = IPP_JOB_PENDING;
 
     ippSetString(job->attrs, &job->reasons, 0, "none");
   }
 
   if (!(printer->type & CUPS_PRINTER_REMOTE) || Classification)
   {
     
 
     if ((attr = ippFindAttribute(job->attrs, "job-sheets",
                                  IPP_TAG_ZERO)) == NULL)
     {
       cupsdLogMessage(CUPSD_LOG_DEBUG,
                       "Adding default job-sheets values \"%s,%s\"...",
                       printer->job_sheets[0], printer->job_sheets[1]);
 
       attr = ippAddStrings(job->attrs, IPP_TAG_JOB, IPP_TAG_NAME, "job-sheets",
                            2, NULL, NULL);
       ippSetString(job->attrs, &attr, 0, printer->job_sheets[0]);
       ippSetString(job->attrs, &attr, 1, printer->job_sheets[1]);
     }
 
     job->job_sheets = attr;
 
     
 
     if (Classification)
     {
       cupsdLogMessage(CUPSD_LOG_INFO,
                       "Classification=\"%s\", ClassifyOverride=%d",
                       Classification ? Classification : "(null)",
 		      ClassifyOverride);
 
       if (ClassifyOverride)
       {
         if (!strcmp(attr->values[0].string.text, "none") &&
 	    (attr->num_values == 1 ||
 	     !strcmp(attr->values[1].string.text, "none")))
         {
 	  
 
           ippSetString(job->attrs, &attr, 0, Classification);
 
 	  cupsdLogJob(job, CUPSD_LOG_NOTICE, "CLASSIFICATION FORCED "
 	                		     "job-sheets=\"%s,none\", "
 					     "job-originating-user-name=\"%s\"",
 	              Classification, job->username);
 	}
 	else if (attr->num_values == 2 &&
 	         strcmp(attr->values[0].string.text,
 		        attr->values[1].string.text) &&
 		 strcmp(attr->values[0].string.text, "none") &&
 		 strcmp(attr->values[1].string.text, "none"))
         {
 	  
 
           ippSetString(job->attrs, &attr, 1, attr->values[0].string.text);
 
 	  cupsdLogJob(job, CUPSD_LOG_NOTICE, "CLASSIFICATION FORCED "
 	                		     "job-sheets=\"%s,%s\", "
 					     "job-originating-user-name=\"%s\"",
 		      attr->values[0].string.text,
 		      attr->values[1].string.text, job->username);
 	}
 	else if (strcmp(attr->values[0].string.text, Classification) &&
 	         strcmp(attr->values[0].string.text, "none") &&
 		 (attr->num_values == 1 ||
 	          (strcmp(attr->values[1].string.text, Classification) &&
 	           strcmp(attr->values[1].string.text, "none"))))
         {
 	  if (attr->num_values == 1)
             cupsdLogJob(job, CUPSD_LOG_NOTICE,
 			"CLASSIFICATION OVERRIDDEN "
 			"job-sheets=\"%s\", "
 			"job-originating-user-name=\"%s\"",
 	                attr->values[0].string.text, job->username);
           else
             cupsdLogJob(job, CUPSD_LOG_NOTICE,
 			"CLASSIFICATION OVERRIDDEN "
 			"job-sheets=\"%s,%s\",fffff "
 			"job-originating-user-name=\"%s\"",
 			attr->values[0].string.text,
 			attr->values[1].string.text, job->username);
         }
       }
       else if (strcmp(attr->values[0].string.text, Classification) &&
                (attr->num_values == 1 ||
 	       strcmp(attr->values[1].string.text, Classification)))
       {
         
 
         if (attr->num_values > 1 &&
 	    !strcmp(attr->values[0].string.text, attr->values[1].string.text))
 	{
           ippSetString(job->attrs, &attr, 0, Classification);
           ippSetString(job->attrs, &attr, 1, Classification);
 	}
         else
 	{
           if (attr->num_values == 1 ||
 	      strcmp(attr->values[0].string.text, "none"))
             ippSetString(job->attrs, &attr, 0, Classification);
 
           if (attr->num_values > 1 &&
 	      strcmp(attr->values[1].string.text, "none"))
 	    ippSetString(job->attrs, &attr, 1, Classification);
         }
 
         if (attr->num_values > 1)
 	  cupsdLogJob(job, CUPSD_LOG_NOTICE,
 		      "CLASSIFICATION FORCED "
 		      "job-sheets=\"%s,%s\", "
 		      "job-originating-user-name=\"%s\"",
 		      attr->values[0].string.text,
 		      attr->values[1].string.text, job->username);
         else
 	  cupsdLogJob(job, CUPSD_LOG_NOTICE,
 		      "CLASSIFICATION FORCED "
 		      "job-sheets=\"%s\", "
 		      "job-originating-user-name=\"%s\"",
 		      Classification, job->username);
       }
     }
 
     
 
     if (!(printer->type & CUPS_PRINTER_REMOTE))
     {
       cupsdLogJob(job, CUPSD_LOG_INFO, "Adding start banner page \"%s\".",
 		  attr->values[0].string.text);
 
       if ((kbytes = copy_banner(con, job, attr->values[0].string.text)) < 0)
       {
         cupsdSetJobState(job, IPP_JOB_ABORTED, CUPSD_JOB_PURGE,
 	                 "Aborting job because the start banner could not be "
 			 "copied.");
         return (NULL);
       }
 
       cupsdUpdateQuota(printer, job->username, 0, kbytes);
     }
   }
   else if ((attr = ippFindAttribute(job->attrs, "job-sheets",
                                     IPP_TAG_ZERO)) != NULL)
     job->job_sheets = attr;
 
   
 
   httpAssembleURIf(HTTP_URI_CODING_ALL, job_uri, sizeof(job_uri), "ipp", NULL,
                    con->clientname, con->clientport, "/jobs/%d", job->id);
   ippAddString(con->response, IPP_TAG_JOB, IPP_TAG_URI, "job-uri", NULL,
                job_uri);
 
   ippAddInteger(con->response, IPP_TAG_JOB, IPP_TAG_INTEGER, "job-id", job->id);
 
   ippAddInteger(con->response, IPP_TAG_JOB, IPP_TAG_ENUM, "job-state",
                 job->state_value);
   ippAddString(con->response, IPP_TAG_JOB, IPP_TAG_TEXT, "job-state-message", NULL, "");
   ippAddString(con->response, IPP_TAG_JOB, IPP_TAG_KEYWORD, "job-state-reasons",
                NULL, job->reasons->values[0].string.text);
 
   con->response->request.status.status_code = IPP_OK;
 
   
 
   add_job_subscriptions(con, job);
 
   
 
   for (attr = job->attrs->attrs->next->next; attr; attr = attr->next)
     attr->group_tag = IPP_TAG_JOB;
 
   
 
   cupsdAddEvent(CUPSD_EVENT_JOB_CREATED, printer, job, "Job created.");
 
   
 
   return (job);
 }