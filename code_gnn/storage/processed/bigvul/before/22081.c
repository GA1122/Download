static CURLcode imap_done(struct connectdata *conn, CURLcode status,
                          bool premature)
{
  struct SessionHandle *data = conn->data;
  struct FTP *imap = data->state.proto.imap;
  CURLcode result=CURLE_OK;
  (void)premature;

  if(!imap)
     
    return CURLE_OK;

  if(status) {
    conn->bits.close = TRUE;  
    result = status;       
  }

   
  imap->transfer = FTPTRANSFER_BODY;

  return result;
}
