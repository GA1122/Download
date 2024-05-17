static CURLcode imap_init(struct connectdata *conn)
{
  struct SessionHandle *data = conn->data;
  struct FTP *imap = data->state.proto.imap;
  if(!imap) {
    imap = data->state.proto.imap = calloc(sizeof(struct FTP), 1);
    if(!imap)
      return CURLE_OUT_OF_MEMORY;
  }

   
  imap->bytecountp = &data->req.bytecount;

   
  imap->user = conn->user;
  imap->passwd = conn->passwd;

  return CURLE_OK;
}
