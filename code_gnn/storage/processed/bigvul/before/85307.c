accept_jobs(cupsd_client_t  *con,	 
            ipp_attribute_t *uri)	 
{
  http_status_t	status;			 
  cups_ptype_t	dtype;			 
  cupsd_printer_t *printer;		 


  cupsdLogMessage(CUPSD_LOG_DEBUG2, "accept_jobs(%p[%d], %s)", con,
                  con->number, uri->values[0].string.text);

  

  if (!cupsdValidateDest(uri->values[0].string.text, &dtype, &printer))
  {
    

    send_ipp_status(con, IPP_NOT_FOUND,
                    _("The printer or class does not exist."));
    return;
  }

  

  if ((status = cupsdCheckPolicy(printer->op_policy_ptr, con, NULL)) != HTTP_OK)
  {
    send_http_error(con, status, printer);
    return;
  }

  

  printer->accepting        = 1;
  printer->state_message[0] = '\0';

  cupsdAddEvent(CUPSD_EVENT_PRINTER_STATE, printer, NULL,
                "Now accepting jobs.");

  if (dtype & CUPS_PRINTER_CLASS)
  {
    cupsdMarkDirty(CUPSD_DIRTY_CLASSES);

    cupsdLogMessage(CUPSD_LOG_INFO, "Class \"%s\" now accepting jobs (\"%s\").",
                    printer->name, get_username(con));
  }
  else
  {
    cupsdMarkDirty(CUPSD_DIRTY_PRINTERS);

    cupsdLogMessage(CUPSD_LOG_INFO,
                    "Printer \"%s\" now accepting jobs (\"%s\").",
                    printer->name, get_username(con));
  }

  

  con->response->request.status.status_code = IPP_OK;
}
