static CURLcode setup_connection_internals(struct connectdata *conn)
{
  const struct Curl_handler * p;
  CURLcode result;
  conn->socktype = SOCK_STREAM;  

   
  p = conn->handler;

  if(p->setup_connection) {
    result = (*p->setup_connection)(conn);

    if(result)
      return result;

    p = conn->handler;               
  }

  if(conn->port < 0)
     
    conn->port = p->defport;

  return CURLE_OK;
}
