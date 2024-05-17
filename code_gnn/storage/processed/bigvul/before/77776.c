CURLcode Curl_setup_conn(struct connectdata *conn,
                         bool *protocol_done)
{
  CURLcode result = CURLE_OK;
  struct Curl_easy *data = conn->data;

  Curl_pgrsTime(data, TIMER_NAMELOOKUP);

  if(conn->handler->flags & PROTOPT_NONETWORK) {
     
    *protocol_done = TRUE;
    return result;
  }
  *protocol_done = FALSE;  

   
  conn->bits.proxy_connect_closed = FALSE;

   
  if(data->set.str[STRING_USERAGENT]) {
    Curl_safefree(conn->allocptr.uagent);
    conn->allocptr.uagent =
      aprintf("User-Agent: %s\r\n", data->set.str[STRING_USERAGENT]);
    if(!conn->allocptr.uagent)
      return CURLE_OUT_OF_MEMORY;
  }

  data->req.headerbytecount = 0;

#ifdef CURL_DO_LINEEND_CONV
  data->state.crlf_conversions = 0;  
#endif  

   
  conn->now = Curl_now();

  if(CURL_SOCKET_BAD == conn->sock[FIRSTSOCKET]) {
    conn->bits.tcpconnect[FIRSTSOCKET] = FALSE;
    result = Curl_connecthost(conn, conn->dns_entry);
    if(result)
      return result;
  }
  else {
    Curl_pgrsTime(data, TIMER_CONNECT);     
    Curl_pgrsTime(data, TIMER_APPCONNECT);  
    conn->bits.tcpconnect[FIRSTSOCKET] = TRUE;
    *protocol_done = TRUE;
    Curl_updateconninfo(conn, conn->sock[FIRSTSOCKET]);
    Curl_verboseconnect(conn);
  }

  conn->now = Curl_now();  
  return result;
}
