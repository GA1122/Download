network_connect_to (const char *proxy, int sock,
                    unsigned long address, int port)
{
    struct t_proxy *ptr_proxy;
    struct sockaddr_in addr;
    struct hostent *hostent;
    char *ip4;
    int ret;
    
    ptr_proxy = NULL;
    if (proxy && proxy[0])
    {
        ptr_proxy = proxy_search (proxy);
        if (!ptr_proxy)
            return 0;
    }
    
    if (ptr_proxy)
    {
        memset (&addr, 0, sizeof (addr));
        addr.sin_addr.s_addr = htonl (address);
        ip4 = inet_ntoa(addr.sin_addr);
        
        memset (&addr, 0, sizeof (addr));
        addr.sin_port = htons (CONFIG_INTEGER(ptr_proxy->options[PROXY_OPTION_PORT]));
        addr.sin_family = AF_INET;
        hostent = gethostbyname (CONFIG_STRING(ptr_proxy->options[PROXY_OPTION_ADDRESS]));
        if (!hostent)
            return 0;
        memcpy(&(addr.sin_addr), *(hostent->h_addr_list), sizeof(struct in_addr));
        ret = connect (sock, (struct sockaddr *) &addr, sizeof (addr));
        if ((ret == -1) && (errno != EINPROGRESS))
            return 0;
        if (!network_pass_proxy (proxy, sock, ip4, port))
            return 0;
    }
    else
    {
        memset (&addr, 0, sizeof (addr));
        addr.sin_port = htons (port);
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = htonl (address);
        ret = connect (sock, (struct sockaddr *) &addr, sizeof (addr));
        if ((ret == -1) && (errno != EINPROGRESS))
            return 0;
    }
    return 1;
}
