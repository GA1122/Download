static CURLcode resolve_server(struct Curl_easy *data,
                               struct connectdata *conn,
                               bool *async)
{
  CURLcode result = CURLE_OK;
  timediff_t timeout_ms = Curl_timeleft(data, NULL, TRUE);

   
  if(conn->bits.reuse)
     
    *async = FALSE;

  else {
     
    int rc;
    struct Curl_dns_entry *hostaddr;

#ifdef USE_UNIX_SOCKETS
    if(conn->unix_domain_socket) {
       
      const char *path = conn->unix_domain_socket;

      hostaddr = calloc(1, sizeof(struct Curl_dns_entry));
      if(!hostaddr)
        result = CURLE_OUT_OF_MEMORY;
      else {
        bool longpath = FALSE;
        hostaddr->addr = Curl_unix2addr(path, &longpath,
                                        conn->abstract_unix_socket);
        if(hostaddr->addr)
          hostaddr->inuse++;
        else {
           
          if(longpath) {
            failf(data, "Unix socket path too long: '%s'", path);
            result = CURLE_COULDNT_RESOLVE_HOST;
          }
          else
            result = CURLE_OUT_OF_MEMORY;
          free(hostaddr);
          hostaddr = NULL;
        }
      }
    }
    else
#endif
    if(!conn->bits.proxy) {
      struct hostname *connhost;
      if(conn->bits.conn_to_host)
        connhost = &conn->conn_to_host;
      else
        connhost = &conn->host;

       
      if(conn->bits.conn_to_port)
        conn->port = conn->conn_to_port;
      else
        conn->port = conn->remote_port;

       
      rc = Curl_resolv_timeout(conn, connhost->name, (int)conn->port,
                               &hostaddr, timeout_ms);
      if(rc == CURLRESOLV_PENDING)
        *async = TRUE;

      else if(rc == CURLRESOLV_TIMEDOUT)
        result = CURLE_OPERATION_TIMEDOUT;

      else if(!hostaddr) {
        failf(data, "Couldn't resolve host '%s'", connhost->dispname);
        result =  CURLE_COULDNT_RESOLVE_HOST;
         
      }
    }
    else {
       

      struct hostname * const host = conn->bits.socksproxy ?
        &conn->socks_proxy.host : &conn->http_proxy.host;

       
      rc = Curl_resolv_timeout(conn, host->name, (int)conn->port,
                               &hostaddr, timeout_ms);

      if(rc == CURLRESOLV_PENDING)
        *async = TRUE;

      else if(rc == CURLRESOLV_TIMEDOUT)
        result = CURLE_OPERATION_TIMEDOUT;

      else if(!hostaddr) {
        failf(data, "Couldn't resolve proxy '%s'", host->dispname);
        result = CURLE_COULDNT_RESOLVE_PROXY;
         
      }
    }
    DEBUGASSERT(conn->dns_entry == NULL);
    conn->dns_entry = hostaddr;
  }

  return result;
}
