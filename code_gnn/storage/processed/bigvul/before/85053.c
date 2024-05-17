static CURLcode smtp_perform_authentication(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  saslprogress progress;

   
  if(!smtpc->auth_supported ||
     !Curl_sasl_can_authenticate(&smtpc->sasl, conn)) {
    state(conn, SMTP_STOP);
    return result;
  }

   
  result = Curl_sasl_start(&smtpc->sasl, conn, FALSE, &progress);

  if(!result) {
    if(progress == SASL_INPROGRESS)
      state(conn, SMTP_AUTH);
    else {
       
      infof(conn->data, "No known authentication mechanisms supported!\n");
      result = CURLE_LOGIN_DENIED;
    }
  }

  return result;
}
