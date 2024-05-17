static CURLcode smtp_state_postdata_resp(struct connectdata *conn,
                                         int smtpcode,
                                         smtpstate instate)
{
  CURLcode result = CURLE_OK;

  (void)instate;  

  if(smtpcode != 250)
    result = CURLE_RECV_ERROR;

   
  state(conn, SMTP_STOP);

  return result;
}