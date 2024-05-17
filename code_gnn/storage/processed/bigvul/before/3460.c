network_pass_proxy (const char *proxy, int sock, const char *address, int port)
{
    int rc;
    struct t_proxy *ptr_proxy;
    
    rc = 0;
    
    ptr_proxy = proxy_search (proxy);
    if (ptr_proxy)
    {
        switch (CONFIG_INTEGER(ptr_proxy->options[PROXY_OPTION_TYPE]))
        {
            case PROXY_TYPE_HTTP:
                rc = network_pass_httpproxy (ptr_proxy, sock, address, port);
                break;
            case PROXY_TYPE_SOCKS4:
                rc = network_pass_socks4proxy (ptr_proxy, sock, address, port);
                break;
            case PROXY_TYPE_SOCKS5:
                rc = network_pass_socks5proxy (ptr_proxy, sock, address, port);
                break;
        }
    }
    return rc;
}
