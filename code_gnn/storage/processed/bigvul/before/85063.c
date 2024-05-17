static CURLcode smtp_setup_connection(struct connectdata *conn)
{
  struct Curl_easy *data = conn->data;
  CURLcode result;

   
  conn->tls_upgraded = FALSE;

   
  result = smtp_init(conn);
  if(result)
    return result;

  data->state.path++;    

  return CURLE_OK;
}
