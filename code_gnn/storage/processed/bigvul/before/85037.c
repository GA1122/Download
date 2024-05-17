static CURLcode smtp_connect(struct connectdata *conn, bool *done)
{
  CURLcode result = CURLE_OK;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  struct pingpong *pp = &smtpc->pp;

  *done = FALSE;  

   
  connkeep(conn, "SMTP default");

   
  pp->response_time = RESP_TIMEOUT;
  pp->statemach_act = smtp_statemach_act;
  pp->endofresp = smtp_endofresp;
  pp->conn = conn;

   
  Curl_sasl_init(&smtpc->sasl, &saslsmtp);

   
  Curl_pp_init(pp);

   
  result = smtp_parse_url_options(conn);
  if(result)
    return result;

   
  result = smtp_parse_url_path(conn);
  if(result)
    return result;

   
  state(conn, SMTP_SERVERGREET);

  result = smtp_multi_statemach(conn, done);

  return result;
}
