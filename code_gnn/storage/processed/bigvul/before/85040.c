static CURLcode smtp_do(struct connectdata *conn, bool *done)
{
  CURLcode result = CURLE_OK;

  *done = FALSE;  

   
  result = smtp_parse_custom_request(conn);
  if(result)
    return result;

  result = smtp_regular_transfer(conn, done);

  return result;
}
