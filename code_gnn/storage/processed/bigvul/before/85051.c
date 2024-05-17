static CURLcode smtp_perform(struct connectdata *conn, bool *connected,
                             bool *dophase_done)
{
   
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;

  DEBUGF(infof(conn->data, "DO phase starts\n"));

  if(data->set.opt_no_body) {
     
    smtp->transfer = FTPTRANSFER_INFO;
  }

  *dophase_done = FALSE;  

   
  smtp->rcpt = data->set.mail_rcpt;

   
  smtp->trailing_crlf = TRUE;
  smtp->eob = 2;

   
  if((data->set.upload || data->set.mimepost.kind) && data->set.mail_rcpt)
     
    result = smtp_perform_mail(conn);
  else
     
    result = smtp_perform_command(conn);

  if(result)
    return result;

   
  result = smtp_multi_statemach(conn, dophase_done);

  *connected = conn->bits.tcpconnect[FIRSTSOCKET];

  if(*dophase_done)
    DEBUGF(infof(conn->data, "DO phase is complete\n"));

  return result;
}
