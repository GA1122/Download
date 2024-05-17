static CURLcode imap_dophase_done(struct connectdata *conn,
                                  bool connected)
{
  struct FTP *imap = conn->data->state.proto.imap;
  (void)connected;

  if(imap->transfer != FTPTRANSFER_BODY)
     
    Curl_setup_transfer(conn, -1, -1, FALSE, NULL, -1, NULL);

  return CURLE_OK;
}