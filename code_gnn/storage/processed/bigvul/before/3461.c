network_pass_socks4proxy (struct t_proxy *proxy, int sock, const char *address,
                          int port)
{
     
    
    struct t_network_socks4 socks4;
    unsigned char buffer[24];
    char ip_addr[NI_MAXHOST];
    
    socks4.version = 4;
    socks4.method = 1;
    socks4.port = htons (port);
    network_resolve (address, ip_addr, NULL);
    socks4.address = inet_addr (ip_addr);
    strncpy (socks4.user, CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME]),
             sizeof (socks4.user) - 1);
    
    send (sock, (char *) &socks4, 8 + strlen (socks4.user) + 1, 0);
    recv (sock, buffer, sizeof (buffer), 0);
    
     
    if ((buffer[0] == 0) && (buffer[1] == 90))
        return 1;
    
     
    return 0;
}
