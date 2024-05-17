parse_socks_client(const uint8_t *data, size_t datalen,
                   int state, char **reason,
                   ssize_t *drain_out)
{
  unsigned int addrlen;
  *drain_out = 0;
  if (datalen < 2)
    return 0;

  switch (state) {
    case PROXY_SOCKS4_WANT_CONNECT_OK:
       
      if (datalen < 8)
        return 0;

      if (data[1] != 0x5a) {
        *reason = tor_strdup(socks4_response_code_to_string(data[1]));
        return -1;
      }

       
      *drain_out = 8;
      return 1;

    case PROXY_SOCKS5_WANT_AUTH_METHOD_NONE:
       
      if (data[1] != 0x00) {
        *reason = tor_strdup("server doesn't support any of our "
                             "available authentication methods");
        return -1;
      }

      log_info(LD_NET, "SOCKS 5 client: continuing without authentication");
      *drain_out = -1;
      return 1;

    case PROXY_SOCKS5_WANT_AUTH_METHOD_RFC1929:
       
      switch (data[1]) {
        case 0x00:
          log_info(LD_NET, "SOCKS 5 client: we have auth details but server "
                            "doesn't require authentication.");
          *drain_out = -1;
          return 1;
        case 0x02:
          log_info(LD_NET, "SOCKS 5 client: need authentication.");
          *drain_out = -1;
          return 2;
         
      }

      *reason = tor_strdup("server doesn't support any of our available "
                           "authentication methods");
      return -1;

    case PROXY_SOCKS5_WANT_AUTH_RFC1929_OK:
       
      if (data[1] != 0x00) {
        *reason = tor_strdup("authentication failed");
        return -1;
      }

      log_info(LD_NET, "SOCKS 5 client: authentication successful.");
      *drain_out = -1;
      return 1;

    case PROXY_SOCKS5_WANT_CONNECT_OK:
       

       
      if (datalen < 4)
        return 0;

      switch (data[3]) {
        case 0x01:  
          addrlen = 4;
          break;
        case 0x04:  
          addrlen = 16;
          break;
        case 0x03:  
          if (datalen < 5)
            return 0;
          addrlen = 1 + data[4];
          break;
        default:
          *reason = tor_strdup("invalid response to connect request");
          return -1;
      }

       
      if (datalen < 6 + addrlen)
        return 0;

      if (data[1] != 0x00) {
        *reason = tor_strdup(socks5_response_code_to_string(data[1]));
        return -1;
      }

      *drain_out = 6 + addrlen;
      return 1;
  }

   
  tor_assert(0);

  return -1;
}
