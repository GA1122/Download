add_printer(cupsd_client_t  *con,	 
            ipp_attribute_t *uri)	 
{
  http_status_t	status;			 
  int		i;			 
  char		scheme[HTTP_MAX_URI],	 
		username[HTTP_MAX_URI],	 
		host[HTTP_MAX_URI],	 
		resource[HTTP_MAX_URI];	 
  int		port;			 
  cupsd_printer_t *printer;		 
  ipp_attribute_t *attr;		 
  cups_file_t	*fp;			 
  char		line[1024];		 
  char		srcfile[1024],		 
		dstfile[1024];		 
  int		modify;			 
  int		changed_driver,		 
		need_restart_job,	 
		set_device_uri,		 
		set_port_monitor;	 


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "add_printer(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

  

  httpSeparateURI(HTTP_URI_CODING_ALL, uri->values[0].string.text, scheme,
                  sizeof(scheme), username, sizeof(username), host,
		  sizeof(host), &port, resource, sizeof(resource));

  if (strncmp(resource, "/printers/", 10) || strlen(resource) == 10)
  {
    

    send_ipp_status(con, IPP_BAD_REQUEST,
                    _("The printer-uri must be of the form "
		      "\"ipp://HOSTNAME/printers/PRINTERNAME\"."));
    return;
  }

  

  if (!validate_name(resource + 10))
  {
    

    send_ipp_status(con, IPP_BAD_REQUEST,
                    _("The printer-uri \"%s\" contains invalid characters."),
		    uri->values[0].string.text);
    return;
  }

  

  if ((printer = cupsdFindPrinter(resource + 10)) == NULL)
  {
    

    if ((printer = cupsdFindClass(resource + 10)) != NULL)
    {
      

      send_ipp_status(con, IPP_NOT_POSSIBLE,
                      _("A class named \"%s\" already exists."),
        	      resource + 10);
      return;
    }

    

    if ((status = cupsdCheckPolicy(DefaultPolicyPtr, con, NULL)) != HTTP_OK)
    {
      send_http_error(con, status, NULL);
      return;
    }

    printer = cupsdAddPrinter(resource + 10);
    modify  = 0;
  }
  else if ((status = cupsdCheckPolicy(printer->op_policy_ptr, con,
                                      NULL)) != HTTP_OK)
  {
    send_http_error(con, status, printer);
    return;
  }
  else
    modify = 1;

  

  changed_driver   = 0;
  need_restart_job = 0;

  if ((attr = ippFindAttribute(con->request, "printer-is-temporary", IPP_TAG_BOOLEAN)) != NULL)
    printer->temporary = ippGetBoolean(attr, 0);

  if ((attr = ippFindAttribute(con->request, "printer-location",
                               IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&printer->location, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-geo-location", IPP_TAG_URI)) != NULL && !strncmp(attr->values[0].string.text, "geo:", 4))
    cupsdSetString(&printer->geo_location, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-organization", IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&printer->organization, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-organizational-unit", IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&printer->organizational_unit, attr->values[0].string.text);

  if ((attr = ippFindAttribute(con->request, "printer-info",
                               IPP_TAG_TEXT)) != NULL)
    cupsdSetString(&printer->info, attr->values[0].string.text);

  set_device_uri = 0;

  if ((attr = ippFindAttribute(con->request, "device-uri",
                               IPP_TAG_URI)) != NULL)
  {
    

    http_uri_status_t	uri_status;	 
    char		old_device_uri[1024];
					 

    need_restart_job = 1;

    uri_status = httpSeparateURI(HTTP_URI_CODING_ALL,
				 attr->values[0].string.text,
				 scheme, sizeof(scheme),
				 username, sizeof(username),
				 host, sizeof(host), &port,
				 resource, sizeof(resource));

    cupsdLogMessage(CUPSD_LOG_DEBUG, "%s device-uri: %s", printer->name, httpURIStatusString(uri_status));

    if (uri_status < HTTP_URI_OK)
    {
      send_ipp_status(con, IPP_NOT_POSSIBLE, _("Bad device-uri \"%s\"."),
		      attr->values[0].string.text);
      if (!modify)
        cupsdDeletePrinter(printer, 0);

      return;
    }

    if (!strcmp(scheme, "file"))
    {
      

      if (!FileDevice && strcmp(resource, "/dev/null"))
      {
        

	send_ipp_status(con, IPP_NOT_POSSIBLE,
	                _("File device URIs have been disabled. "
	                  "To enable, see the FileDevice directive in "
			  "\"%s/cups-files.conf\"."),
			ServerRoot);
	if (!modify)
	  cupsdDeletePrinter(printer, 0);

	return;
      }
    }
    else
    {
      

      snprintf(srcfile, sizeof(srcfile), "%s/backend/%s", ServerBin, scheme);
      if (access(srcfile, X_OK))
      {
        

	send_ipp_status(con, IPP_NOT_POSSIBLE,
                        _("Bad device-uri scheme \"%s\"."), scheme);
	if (!modify)
	  cupsdDeletePrinter(printer, 0);

	return;
      }
    }

    if (printer->sanitized_device_uri)
      strlcpy(old_device_uri, printer->sanitized_device_uri,
              sizeof(old_device_uri));
    else
      old_device_uri[0] = '\0';

    cupsdSetDeviceURI(printer, attr->values[0].string.text);

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Setting %s device-uri to \"%s\" (was \"%s\".)",
        	    printer->name, printer->sanitized_device_uri,
		    old_device_uri);

    set_device_uri = 1;
  }

  set_port_monitor = 0;

  if ((attr = ippFindAttribute(con->request, "port-monitor",
                               IPP_TAG_NAME)) != NULL)
  {
    ipp_attribute_t	*supported;	 


    need_restart_job = 1;

    supported = ippFindAttribute(printer->ppd_attrs, "port-monitor-supported",
                                 IPP_TAG_NAME);
    if (supported)
    {
      for (i = 0; i < supported->num_values; i ++)
        if (!strcmp(supported->values[i].string.text,
                    attr->values[0].string.text))
          break;
    }

    if (!supported || i >= supported->num_values)
    {
      send_ipp_status(con, IPP_NOT_POSSIBLE, _("Bad port-monitor \"%s\"."),
        	      attr->values[0].string.text);
      if (!modify)
        cupsdDeletePrinter(printer, 0);

      return;
    }

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Setting %s port-monitor to \"%s\" (was \"%s\".)",
                    printer->name, attr->values[0].string.text,
	            printer->port_monitor ? printer->port_monitor : "none");

    if (strcmp(attr->values[0].string.text, "none"))
      cupsdSetString(&printer->port_monitor, attr->values[0].string.text);
    else
      cupsdClearString(&printer->port_monitor);

    set_port_monitor = 1;
  }

  if ((attr = ippFindAttribute(con->request, "printer-is-accepting-jobs",
                               IPP_TAG_BOOLEAN)) != NULL &&
      attr->values[0].boolean != printer->accepting)
  {
    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Setting %s printer-is-accepting-jobs to %d (was %d.)",
                    printer->name, attr->values[0].boolean, printer->accepting);

    printer->accepting = attr->values[0].boolean;

    cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, printer, NULL,
                  "%s accepting jobs.",
		  printer->accepting ? "Now" : "No longer");
  }

  if ((attr = ippFindAttribute(con->request, "printer-is-shared", IPP_TAG_BOOLEAN)) != NULL)
  {
    if (ippGetBoolean(attr, 0) &&
        printer->num_auth_info_required == 1 &&
	!strcmp(printer->auth_info_required[0], "negotiate"))
    {
      send_ipp_status(con, IPP_BAD_REQUEST,
                      _("Cannot share a remote Kerberized printer."));
      if (!modify)
        cupsdDeletePrinter(printer, 0);

      return;
    }

    if (printer->type & CUPS_PRINTER_REMOTE)
    {
      

      send_ipp_status(con, IPP_BAD_REQUEST, _("Cannot change printer-is-shared for remote queues."));
      if (!modify)
        cupsdDeletePrinter(printer, 0);

      return;
    }

    if (printer->shared && !ippGetBoolean(attr, 0))
      cupsdDeregisterPrinter(printer, 1);

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Setting %s printer-is-shared to %d (was %d.)",
                    printer->name, attr->values[0].boolean, printer->shared);

    printer->shared = ippGetBoolean(attr, 0);
    if (printer->shared && printer->temporary)
      printer->temporary = 0;
  }

  if ((attr = ippFindAttribute(con->request, "printer-state",
                               IPP_TAG_ENUM)) != NULL)
  {
    if (attr->values[0].integer != IPP_PRINTER_IDLE &&
        attr->values[0].integer != IPP_PRINTER_STOPPED)
    {
      send_ipp_status(con, IPP_BAD_REQUEST, _("Bad printer-state value %d."),
                      attr->values[0].integer);
      if (!modify)
        cupsdDeletePrinter(printer, 0);

      return;
    }

    cupsdLogMessage(CUPSD_LOG_INFO, "Setting %s printer-state to %d (was %d.)",
                    printer->name, attr->values[0].integer, printer->state);

    if (attr->values[0].integer == IPP_PRINTER_STOPPED)
      cupsdStopPrinter(printer, 0);
    else
    {
      need_restart_job = 1;
      cupsdSetPrinterState(printer, (ipp_pstate_t)(attr->values[0].integer), 0);
    }
  }

  if ((attr = ippFindAttribute(con->request, "printer-state-message",
                               IPP_TAG_TEXT)) != NULL)
  {
    strlcpy(printer->state_message, attr->values[0].string.text,
            sizeof(printer->state_message));

    cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, printer, NULL, "%s",
                  printer->state_message);
  }

  if ((attr = ippFindAttribute(con->request, "printer-state-reasons",
                               IPP_TAG_KEYWORD)) != NULL)
  {
    if (attr->num_values >
            (int)(sizeof(printer->reasons) / sizeof(printer->reasons[0])))
    {
      send_ipp_status(con, IPP_NOT_POSSIBLE,
                      _("Too many printer-state-reasons values (%d > %d)."),
		      attr->num_values,
		      (int)(sizeof(printer->reasons) /
		            sizeof(printer->reasons[0])));
      if (!modify)
        cupsdDeletePrinter(printer, 0);

      return;
    }

    for (i = 0; i < printer->num_reasons; i ++)
      _cupsStrFree(printer->reasons[i]);

    printer->num_reasons = 0;
    for (i = 0; i < attr->num_values; i ++)
    {
      if (!strcmp(attr->values[i].string.text, "none"))
        continue;

      printer->reasons[printer->num_reasons] =
          _cupsStrRetain(attr->values[i].string.text);
      printer->num_reasons ++;

      if (!strcmp(attr->values[i].string.text, "paused") &&
          printer->state != IPP_PRINTER_STOPPED)
      {
	cupsdLogMessage(CUPSD_LOG_INFO,
	                "Setting %s printer-state to %d (was %d.)",
			printer->name, IPP_PRINTER_STOPPED, printer->state);
	cupsdStopPrinter(printer, 0);
      }
    }

    if (PrintcapFormat == PRINTCAP_PLIST)
      cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);

    cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, printer, NULL,
                  "Printer \"%s\" state changed.", printer->name);
  }

  if (!set_printer_defaults(con, printer))
  {
    if (!modify)
      cupsdDeletePrinter(printer, 0);

    return;
  }

  if ((attr = ippFindAttribute(con->request, "auth-info-required",
                               IPP_TAG_KEYWORD)) != NULL)
    cupsdSetAuthInfoRequired(printer, NULL, attr);

  

  if (!printer->device_uri)
    cupsdSetString(&printer->device_uri, "file:///dev/null");

  

  if (con->filename)
  {
    need_restart_job = 1;
    changed_driver   = 1;

    strlcpy(srcfile, con->filename, sizeof(srcfile));

    if ((fp = cupsFileOpen(srcfile, "rb")))
    {
      

      line[0] = '\0';
      cupsFileGets(fp, line, sizeof(line));
      cupsFileClose(fp);

      

      if (strncmp(line, "*PPD-Adobe", 10))
      {
	send_ipp_status(con, IPP_STATUS_ERROR_DOCUMENT_FORMAT_NOT_SUPPORTED, _("Bad PPD file."));
	if (!modify)
	  cupsdDeletePrinter(printer, 0);

	return;
      }

      snprintf(dstfile, sizeof(dstfile), "%s/ppd/%s.ppd", ServerRoot,
               printer->name);

      

      if (copy_file(srcfile, dstfile, ConfigFilePerm))
      {
	send_ipp_status(con, IPP_INTERNAL_ERROR, _("Unable to copy PPD file - %s"), strerror(errno));
	if (!modify)
	  cupsdDeletePrinter(printer, 0);

	return;
      }

      cupsdLogMessage(CUPSD_LOG_DEBUG, "Copied PPD file successfully");
    }
  }
  else if ((attr = ippFindAttribute(con->request, "ppd-name", IPP_TAG_NAME)) != NULL)
  {
    const char *ppd_name = ippGetString(attr, 0, NULL);
					 

    need_restart_job = 1;
    changed_driver   = 1;

    if (!strcmp(ppd_name, "raw"))
    {
      

      snprintf(dstfile, sizeof(dstfile), "%s/ppd/%s.ppd", ServerRoot, printer->name);
      unlink(dstfile);
    }
    else if (strstr(ppd_name, "../"))
    {
      send_ipp_status(con, IPP_STATUS_ERROR_ATTRIBUTES_OR_VALUES, _("Invalid ppd-name value."));
      if (!modify)
	cupsdDeletePrinter(printer, 0);

      return;
    }
    else
    {
      

      snprintf(dstfile, sizeof(dstfile), "%s/ppd/%s.ppd", ServerRoot, printer->name);

      if (copy_model(con, ppd_name, dstfile))
      {
        send_ipp_status(con, IPP_INTERNAL_ERROR, _("Unable to copy PPD file."));
	if (!modify)
	  cupsdDeletePrinter(printer, 0);

	return;
      }

      cupsdLogMessage(CUPSD_LOG_DEBUG, "Copied PPD file successfully");
    }
  }

  if (changed_driver)
  {
    

    char cache_name[1024];		 

    snprintf(cache_name, sizeof(cache_name), "%s/%s.data", CacheDir, printer->name);
    unlink(cache_name);

    cupsdSetPrinterReasons(printer, "none");

    

    cupsdRegisterColor(printer);
  }

  

  if (set_device_uri && !set_port_monitor)
  {
    ppd_file_t	*ppd;			 
    ppd_attr_t	*ppdattr;		 


    httpSeparateURI(HTTP_URI_CODING_ALL, printer->device_uri, scheme,
                    sizeof(scheme), username, sizeof(username), host,
		    sizeof(host), &port, resource, sizeof(resource));

    snprintf(srcfile, sizeof(srcfile), "%s/ppd/%s.ppd", ServerRoot,
	     printer->name);
    if ((ppd = _ppdOpenFile(srcfile, _PPD_LOCALIZATION_NONE)) != NULL)
    {
      for (ppdattr = ppdFindAttr(ppd, "cupsPortMonitor", NULL);
	   ppdattr;
	   ppdattr = ppdFindNextAttr(ppd, "cupsPortMonitor", NULL))
        if (!strcmp(scheme, ppdattr->spec))
	{
	  cupsdLogMessage(CUPSD_LOG_INFO,
			  "Setting %s port-monitor to \"%s\" (was \"%s\".)",
			  printer->name, ppdattr->value,
			  printer->port_monitor ? printer->port_monitor
			                        : "none");

	  if (strcmp(ppdattr->value, "none"))
	    cupsdSetString(&printer->port_monitor, ppdattr->value);
	  else
	    cupsdClearString(&printer->port_monitor);

	  break;
	}

      ppdClose(ppd);
    }
  }

  printer->config_time = time(NULL);

  

  cupsdSetPrinterAttrs(printer);
  if (!printer->temporary)
    cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

  if (need_restart_job && printer->job)
  {
    

    cupsdSetJobState(printer->job, IPP_JOB_PENDING, CUPSD_JOB_FORCE,
                     "Job restarted because the printer was modified.");
  }

  cupsdMarkDirty(CUPSD_DIRTY_PRINTCAP);

  if (modify)
  {
    cupsdAddEvent(CUPSD_EVENT_PRINTER_MODIFIED,
                  printer, NULL, "Printer \"%s\" modified by \"%s\".",
		  printer->name, get_username(con));

    cupsdLogMessage(CUPSD_LOG_INFO, "Printer \"%s\" modified by \"%s\".",
                    printer->name, get_username(con));
  }
  else
  {
    cupsdAddEvent(CUPSD_EVENT_PRINTER_ADDED,
                  printer, NULL, "New printer \"%s\" added by \"%s\".",
		  printer->name, get_username(con));

    cupsdLogMessage(CUPSD_LOG_INFO, "New printer \"%s\" added by \"%s\".",
                    printer->name, get_username(con));
  }

  con->response->request.status.status_code = IPP_OK;
}
