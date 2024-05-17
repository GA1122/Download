static CURLcode parse_proxy(struct Curl_easy *data,
                            struct connectdata *conn, char *proxy,
                            curl_proxytype proxytype)
{
  char *prox_portno;
  char *endofprot;

   
  char *proxyptr;
  char *portptr;
  char *atsign;
  long port = -1;
  char *proxyuser = NULL;
  char *proxypasswd = NULL;
  bool sockstype;

   

   
  endofprot = strstr(proxy, "://");
  if(endofprot) {
    proxyptr = endofprot + 3;
    if(checkprefix("https", proxy))
      proxytype = CURLPROXY_HTTPS;
    else if(checkprefix("socks5h", proxy))
      proxytype = CURLPROXY_SOCKS5_HOSTNAME;
    else if(checkprefix("socks5", proxy))
      proxytype = CURLPROXY_SOCKS5;
    else if(checkprefix("socks4a", proxy))
      proxytype = CURLPROXY_SOCKS4A;
    else if(checkprefix("socks4", proxy) || checkprefix("socks", proxy))
      proxytype = CURLPROXY_SOCKS4;
    else if(checkprefix("http:", proxy))
      ;  
    else {
       
      failf(data, "Unsupported proxy scheme for \'%s\'", proxy);
      return CURLE_COULDNT_CONNECT;
    }
  }
  else
    proxyptr = proxy;  

#ifdef USE_SSL
  if(!(Curl_ssl->supports & SSLSUPP_HTTPS_PROXY))
#endif
    if(proxytype == CURLPROXY_HTTPS) {
      failf(data, "Unsupported proxy \'%s\', libcurl is built without the "
                  "HTTPS-proxy support.", proxy);
      return CURLE_NOT_BUILT_IN;
    }

  sockstype = proxytype == CURLPROXY_SOCKS5_HOSTNAME ||
              proxytype == CURLPROXY_SOCKS5 ||
              proxytype == CURLPROXY_SOCKS4A ||
              proxytype == CURLPROXY_SOCKS4;

   
  atsign = strchr(proxyptr, '@');
  if(atsign) {
    CURLcode result =
      Curl_parse_login_details(proxyptr, atsign - proxyptr,
                               &proxyuser, &proxypasswd, NULL);
    if(result)
      return result;
    proxyptr = atsign + 1;
  }

   
  portptr = proxyptr;

   
  if(*proxyptr == '[') {
    char *ptr = ++proxyptr;  
    while(*ptr && (ISXDIGIT(*ptr) || (*ptr == ':') || (*ptr == '.')))
      ptr++;
    if(*ptr == '%') {
       
      if(strncmp("%25", ptr, 3))
        infof(data, "Please URL encode %% as %%25, see RFC 6874.\n");
      ptr++;
       
      while(*ptr && (ISALPHA(*ptr) || ISXDIGIT(*ptr) || (*ptr == '-') ||
                     (*ptr == '.') || (*ptr == '_') || (*ptr == '~')))
        ptr++;
    }
    if(*ptr == ']')
       
      *ptr++ = 0;
    else
      infof(data, "Invalid IPv6 address format\n");
    portptr = ptr;
     
  }

   
  prox_portno = strchr(portptr, ':');
  if(prox_portno) {
    char *endp = NULL;

    *prox_portno = 0x0;  
    prox_portno ++;
     
    port = strtol(prox_portno, &endp, 10);
    if((endp && *endp && (*endp != '/') && (*endp != ' ')) ||
       (port < 0) || (port > 65535)) {
       
      infof(data, "No valid port number in proxy string (%s)\n",
            prox_portno);
    }
    else
      conn->port = port;
  }
  else {
    if(proxyptr[0]=='/') {
       
      Curl_safefree(proxyuser);
      Curl_safefree(proxypasswd);
      return CURLE_COULDNT_RESOLVE_PROXY;
    }

     
    atsign = strchr(proxyptr, '/');
    if(atsign)
      *atsign = '\0';  

    if(data->set.proxyport)
       
      port = data->set.proxyport;
    else {
      if(proxytype == CURLPROXY_HTTPS)
        port = CURL_DEFAULT_HTTPS_PROXY_PORT;
      else
        port = CURL_DEFAULT_PROXY_PORT;
    }
  }

  if(*proxyptr) {
    struct proxy_info *proxyinfo =
      sockstype ? &conn->socks_proxy : &conn->http_proxy;
    proxyinfo->proxytype = proxytype;

    if(proxyuser) {
       
      Curl_safefree(proxyinfo->user);
      proxyinfo->user = curl_easy_unescape(data, proxyuser, 0, NULL);
      Curl_safefree(proxyuser);

      if(!proxyinfo->user) {
        Curl_safefree(proxypasswd);
        return CURLE_OUT_OF_MEMORY;
      }

      Curl_safefree(proxyinfo->passwd);
      if(proxypasswd && strlen(proxypasswd) < MAX_CURL_PASSWORD_LENGTH)
        proxyinfo->passwd = curl_easy_unescape(data, proxypasswd, 0, NULL);
      else
        proxyinfo->passwd = strdup("");
      Curl_safefree(proxypasswd);

      if(!proxyinfo->passwd)
        return CURLE_OUT_OF_MEMORY;

      conn->bits.proxy_user_passwd = TRUE;  
    }

    if(port >= 0) {
      proxyinfo->port = port;
      if(conn->port < 0 || sockstype || !conn->socks_proxy.host.rawalloc)
        conn->port = port;
    }

     
    Curl_safefree(proxyinfo->host.rawalloc);
    proxyinfo->host.rawalloc = strdup(proxyptr);
    proxyinfo->host.name = proxyinfo->host.rawalloc;

    if(!proxyinfo->host.rawalloc)
      return CURLE_OUT_OF_MEMORY;
  }

  Curl_safefree(proxyuser);
  Curl_safefree(proxypasswd);

  return CURLE_OK;
}
