static CURLcode findprotocol(struct Curl_easy *data,
                             struct connectdata *conn,
                             const char *protostr)
{
  const struct Curl_handler *p = Curl_builtin_scheme(protostr);

  if(p &&  
     (data->set.allowed_protocols & p->protocol)) {

     
    if(data->state.this_is_a_follow &&
       !(data->set.redir_protocols & p->protocol))
       
      ;
    else {
       
      conn->handler = conn->given = p;

       
      return CURLE_OK;
    }
  }

   
  failf(data, "Protocol \"%s\" not supported or disabled in " LIBCURL_NAME,
        protostr);

  return CURLE_UNSUPPORTED_PROTOCOL;
}
