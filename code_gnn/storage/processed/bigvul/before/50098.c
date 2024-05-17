static CURLcode nss_connect_common(struct connectdata *conn, int sockindex,
                                   bool *done)
{
  struct ssl_connect_data *connssl = &conn->ssl[sockindex];
  struct Curl_easy *data = conn->data;
  const bool blocking = (done == NULL);
  CURLcode result;

  if(connssl->state == ssl_connection_complete)
    return CURLE_OK;

  if(connssl->connecting_state == ssl_connect_1) {
    result = nss_setup_connect(conn, sockindex);
    if(result)
       
      return result;

    if(!blocking) {
       
      result = nss_set_nonblock(connssl, data);
      if(result)
        return result;
    }

    connssl->connecting_state = ssl_connect_2;
  }

  result = nss_do_connect(conn, sockindex);
  switch(result) {
  case CURLE_OK:
    break;
  case CURLE_AGAIN:
    if(!blocking)
       
      return CURLE_OK;
     
  default:
    return result;
  }

  if(blocking) {
     
    result = nss_set_nonblock(connssl, data);
    if(result)
      return result;
  }
  else
     
    *done = TRUE;

  connssl->state = ssl_connection_complete;
  conn->recv[sockindex] = nss_recv;
  conn->send[sockindex] = nss_send;

   
  connssl->connecting_state = ssl_connect_1;

  return CURLE_OK;
}
