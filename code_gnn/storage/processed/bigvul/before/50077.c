int Curl_nss_random(struct Curl_easy *data,
                    unsigned char *entropy,
                    size_t length)
{
  Curl_nss_seed(data);   

  if(SECSuccess != PK11_GenerateRandom(entropy, curlx_uztosi(length)))
     
    return -1;

  return 0;
}
