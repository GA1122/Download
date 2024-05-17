static CURLcode imap_disconnect(struct connectdata *conn, bool dead_connection)
{
  struct imap_conn *imapc= &conn->proto.imapc;

   
  if(!dead_connection && imapc->pp.conn)
    (void)imap_logout(conn);  

  Curl_pp_disconnect(&imapc->pp);

  Curl_safefree(imapc->mailbox);

  return CURLE_OK;
}
