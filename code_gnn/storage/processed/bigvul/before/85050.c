static CURLcode smtp_parse_url_path(struct connectdata *conn)
{
   
  struct Curl_easy *data = conn->data;
  struct smtp_conn *smtpc = &conn->proto.smtpc;
  const char *path = data->state.path;
  char localhost[HOSTNAME_MAX + 1];

   
  if(!*path) {
    if(!Curl_gethostname(localhost, sizeof(localhost)))
      path = localhost;
    else
      path = "localhost";
  }

   
  return Curl_urldecode(conn->data, path, 0, &smtpc->domain, NULL, TRUE);
}
