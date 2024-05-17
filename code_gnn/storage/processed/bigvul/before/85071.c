static CURLcode smtp_state_rcpt_resp(struct connectdata *conn, int smtpcode,
                                     smtpstate instate)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;
  struct SMTP *smtp = data->req.protop;

  (void)instate;  

  if(smtpcode/100 != 2) {
    failf(data, "RCPT failed: %d", smtpcode);
    result = CURLE_SEND_ERROR;
  }
  else {
    smtp->rcpt = smtp->rcpt->next;

    if(smtp->rcpt)
       
      result = smtp_perform_rcpt_to(conn);
    else {
       
      result = Curl_pp_sendf(&conn->proto.smtpc.pp, "%s", "DATA");

      if(!result)
        state(conn, SMTP_DATA);
    }
  }

  return result;
}