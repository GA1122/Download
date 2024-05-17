CURLcode Curl_connect(struct Curl_easy *data,
                      struct connectdata **in_connect,
                      bool *asyncp,
                      bool *protocol_done)
{
  CURLcode result;

  *asyncp = FALSE;  

   
  Curl_free_request_state(data);
  memset(&data->req, 0, sizeof(struct SingleRequest));
  data->req.maxdownload = -1;

   
  result = create_conn(data, in_connect, asyncp);

  if(!result) {
    if(CONN_INUSE(*in_connect))
       
      *protocol_done = TRUE;
    else if(!*asyncp) {
       
      result = Curl_setup_conn(*in_connect, protocol_done);
    }
  }

  if(result == CURLE_NO_CONNECTION_AVAILABLE) {
    *in_connect = NULL;
    return result;
  }
  else if(result && *in_connect) {
     
    Curl_disconnect(data, *in_connect, TRUE);
    *in_connect = NULL;  
  }

  return result;
}
