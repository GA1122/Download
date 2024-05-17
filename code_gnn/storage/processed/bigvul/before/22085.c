static CURLcode imap_fetch(struct connectdata *conn)
{
  CURLcode result = CURLE_OK;
  const char *str;

  str = getcmdid(conn);

   
  result = imapsendf(conn, str, "%s FETCH 1 BODY[TEXT]", str);
  if(result)
    return result;

   

  state(conn, IMAP_FETCH);
  return result;
}
