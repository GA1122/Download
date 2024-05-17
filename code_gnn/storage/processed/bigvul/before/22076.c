static const char *getcmdid(struct connectdata *conn)
{
  static const char * const ids[]= {
    "A",
    "B",
    "C",
    "D"
  };

  struct imap_conn *imapc = &conn->proto.imapc;

   
  imapc->cmdid = (int)((imapc->cmdid+1) % (sizeof(ids)/sizeof(ids[0])));

  return ids[imapc->cmdid];
}
