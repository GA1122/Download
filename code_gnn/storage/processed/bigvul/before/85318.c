cancel_subscription(
    cupsd_client_t *con,		 
    int            sub_id)		 
{
  http_status_t		status;		 
  cupsd_subscription_t	*sub;		 


  cupsdLogMessage(CUPSD_LOG_DEBUG2,
                  "cancel_subscription(con=%p[%d], sub_id=%d)",
                  con, con->number, sub_id);

  

  if ((sub = cupsdFindSubscription(sub_id)) == NULL)
  {
    

    send_ipp_status(con, IPP_NOT_FOUND,
                    _("Subscription #%d does not exist."), sub_id);
    return;
  }

  

  if ((status = cupsdCheckPolicy(sub->dest ? sub->dest->op_policy_ptr :
                                             DefaultPolicyPtr,
                                 con, sub->owner)) != HTTP_OK)
  {
    send_http_error(con, status, sub->dest);
    return;
  }

  

  cupsdDeleteSubscription(sub, 1);

  con->response->request.status.status_code = IPP_OK;
}
