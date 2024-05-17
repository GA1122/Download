static CURLcode smtp_regular_transfer(struct connectdata *conn,
                                      bool *dophase_done)
{
  CURLcode result = CURLE_OK;
  bool connected = FALSE;
  struct Curl_easy *data = conn->data;

   
  data->req.size = -1;

   
  Curl_pgrsSetUploadCounter(data, 0);
  Curl_pgrsSetDownloadCounter(data, 0);
  Curl_pgrsSetUploadSize(data, -1);
  Curl_pgrsSetDownloadSize(data, -1);

   
  result = smtp_perform(conn, &connected, dophase_done);

   
  if(!result && *dophase_done)
    result = smtp_dophase_done(conn, connected);

  return result;
}
