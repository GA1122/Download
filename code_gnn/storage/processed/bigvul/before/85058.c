static CURLcode smtp_perform_quit(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;

   
  result = Curl_pp_sendf(&conn->proto.smtpc.pp, "%s", "QUIT");

  if(!result)
    state(conn, SMTP_QUIT);

  return result;
}