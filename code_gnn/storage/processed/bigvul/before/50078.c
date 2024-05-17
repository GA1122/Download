int Curl_nss_seed(struct Curl_easy *data)
{
   
  return !!Curl_nss_force_init(data);
}
