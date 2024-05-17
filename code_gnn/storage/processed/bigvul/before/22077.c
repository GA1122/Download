static CURLcode imap_connect(struct connectdata *conn,
                                 bool *done)  
{
  CURLcode result;
  struct imap_conn *imapc = &conn->proto.imapc;
  struct SessionHandle *data=conn->data;
  struct pingpong *pp = &imapc->pp;

  *done = FALSE;  

   
  Curl_reset_reqproto(conn);

  result = imap_init(conn);
  if(CURLE_OK != result)
    return result;

   
  conn->bits.close = FALSE;

  pp->response_time = RESP_TIMEOUT;  
  pp->statemach_act = imap_statemach_act;
  pp->endofresp = imap_endofresp;
  pp->conn = conn;

  if(conn->bits.tunnel_proxy && conn->bits.httpproxy) {
     
    struct HTTP http_proxy;
    struct FTP *imap_save;

     
     

     
    imap_save = data->state.proto.imap;
    memset(&http_proxy, 0, sizeof(http_proxy));
    data->state.proto.http = &http_proxy;

    result = Curl_proxyCONNECT(conn, FIRSTSOCKET,
                               conn->host.name, conn->remote_port);

    data->state.proto.imap = imap_save;

    if(CURLE_OK != result)
      return result;
  }

  if((conn->handler->flags & PROTOPT_SSL) &&
     data->state.used_interface != Curl_if_multi) {
     
    result = Curl_ssl_connect(conn, FIRSTSOCKET);
    if(result)
      return result;
  }

  Curl_pp_init(pp);  

   
  state(conn, IMAP_SERVERGREET);
  imapc->idstr = "*";  

  if(data->state.used_interface == Curl_if_multi)
    result = imap_multi_statemach(conn, done);
  else {
    result = imap_easy_statemach(conn);
    if(!result)
      *done = TRUE;
  }

  return result;
}
