static CURLcode smtp_disconnect(struct connectdata *conn, bool dead_connection)
{
  struct smtp_conn *smtpc = &conn->proto.smtpc;

   

   
  if(!dead_connection && smtpc->pp.conn && smtpc->pp.conn->bits.protoconnstart)
    if(!smtp_perform_quit(conn))
      (void)smtp_block_statemach(conn);  

   
  Curl_pp_disconnect(&smtpc->pp);

   
  Curl_sasl_cleanup(conn, smtpc->sasl.authused);

   
  Curl_safefree(smtpc->domain);

  return CURLE_OK;
}
