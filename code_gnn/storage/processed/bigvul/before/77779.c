CURLcode Curl_upkeep(struct conncache *conn_cache,
                          void *data)
{
   
  Curl_conncache_foreach(data,
                         conn_cache,
                         data,
                         conn_upkeep);
  return CURLE_OK;
}
