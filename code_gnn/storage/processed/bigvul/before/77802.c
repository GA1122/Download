static CURLcode parse_connect_to_host_port(struct Curl_easy *data,
                                           const char *host,
                                           char **hostname_result,
                                           int *port_result)
{
  char *host_dup;
  char *hostptr;
  char *host_portno;
  char *portptr;
  int port = -1;

#if defined(CURL_DISABLE_VERBOSE_STRINGS)
  (void) data;
#endif

  *hostname_result = NULL;
  *port_result = -1;

  if(!host || !*host)
    return CURLE_OK;

  host_dup = strdup(host);
  if(!host_dup)
    return CURLE_OUT_OF_MEMORY;

  hostptr = host_dup;

   
  portptr = hostptr;

   
  if(*hostptr == '[') {
#ifdef ENABLE_IPV6
    char *ptr = ++hostptr;  
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
       
      *ptr++ = '\0';
    else
      infof(data, "Invalid IPv6 address format\n");
    portptr = ptr;
     
#else
    failf(data, "Use of IPv6 in *_CONNECT_TO without IPv6 support built-in!");
    free(host_dup);
    return CURLE_NOT_BUILT_IN;
#endif
  }

   
  host_portno = strchr(portptr, ':');
  if(host_portno) {
    char *endp = NULL;
    *host_portno = '\0';  
    host_portno++;
    if(*host_portno) {
      long portparse = strtol(host_portno, &endp, 10);
      if((endp && *endp) || (portparse < 0) || (portparse > 65535)) {
        infof(data, "No valid port number in connect to host string (%s)\n",
              host_portno);
        hostptr = NULL;
        port = -1;
      }
      else
        port = (int)portparse;  
    }
  }

   
  if(hostptr) {
    *hostname_result = strdup(hostptr);
    if(!*hostname_result) {
      free(host_dup);
      return CURLE_OUT_OF_MEMORY;
    }
  }

  *port_result = port;

  free(host_dup);
  return CURLE_OK;
}
