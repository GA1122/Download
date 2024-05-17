network_pass_httpproxy (struct t_proxy *proxy, int sock, const char *address,
                        int port)
{
    char buffer[256], authbuf[128], authbuf_base64[196];
    int n, m;
    
    if (CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME])
        && CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME])[0])
    {
         
        snprintf (authbuf, sizeof (authbuf), "%s:%s",
                  CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME]),
                  (CONFIG_STRING(proxy->options[PROXY_OPTION_PASSWORD])) ?
                  CONFIG_STRING(proxy->options[PROXY_OPTION_PASSWORD]) : "");
        string_encode_base64 (authbuf, strlen (authbuf), authbuf_base64);
        n = snprintf (buffer, sizeof (buffer),
                      "CONNECT %s:%d HTTP/1.0\r\nProxy-Authorization: Basic %s\r\n\r\n",
                      address, port, authbuf_base64);
    }
    else
    {
         
        n = snprintf (buffer, sizeof (buffer),
                      "CONNECT %s:%d HTTP/1.0\r\n\r\n", address, port);
    }
    
    m = send (sock, buffer, n, 0);
    if (n != m)
        return 0;
    
    n = recv (sock, buffer, sizeof (buffer), 0);
    
     
    if (n < 12)
        return 0;
    
    if (memcmp (buffer, "HTTP/", 5) || memcmp (buffer + 9, "200", 3))
        return 0;
    
     
    return 1;
}
