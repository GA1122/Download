CURLcode Curl_init_do(struct Curl_easy *data, struct connectdata *conn)
{
  struct SingleRequest *k = &data->req;

  if(conn) {
    conn->bits.do_more = FALSE;  
     
    if(data->state.wildcardmatch &&
       !(conn->handler->flags & PROTOPT_WILDCARD))
      data->state.wildcardmatch = FALSE;
  }

  data->state.done = FALSE;  
  data->state.expect100header = FALSE;


  if(data->set.opt_no_body)
     
    data->set.httpreq = HTTPREQ_HEAD;
  else if(HTTPREQ_HEAD == data->set.httpreq)
     
    data->set.httpreq = HTTPREQ_GET;

  k->start = Curl_now();  
  k->now = k->start;    
  k->header = TRUE;  

  k->bytecount = 0;

  k->buf = data->state.buffer;
  k->hbufp = data->state.headerbuff;
  k->ignorebody = FALSE;

  Curl_speedinit(data);

  Curl_pgrsSetUploadCounter(data, 0);
  Curl_pgrsSetDownloadCounter(data, 0);

  return CURLE_OK;
}
