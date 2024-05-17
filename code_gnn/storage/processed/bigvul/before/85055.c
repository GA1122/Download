static CURLcode smtp_perform_ehlo(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;

  smtpc->sasl.authmechs = SASL_AUTH_NONE;  
  smtpc->sasl.authused = SASL_AUTH_NONE;   
  smtpc->tls_supported = FALSE;            
  smtpc->auth_supported = FALSE;           

   
  result = Curl_pp_sendf(&smtpc->pp, "EHLO %s", smtpc->domain);

  if(!result)
    state(conn, SMTP_EHLO);

  return result;
}
