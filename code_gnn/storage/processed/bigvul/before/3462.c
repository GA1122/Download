network_pass_socks5proxy (struct t_proxy *proxy, int sock, const char *address,
                          int port)
{
     
    
    struct t_network_socks5 socks5;
    unsigned char buffer[288];
    int username_len, password_len, addr_len, addr_buffer_len;
    unsigned char *addr_buffer;
    
    socks5.version = 5;
    socks5.nmethods = 1;
    
    if (CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME])
        && CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME])[0])
        socks5.method = 2;  
    else
        socks5.method = 0;  
    
    send (sock, (char *) &socks5, sizeof(socks5), 0);
     
    if (recv (sock, buffer, 2, 0) != 2)
        return 0;
    
    if (CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME])
        && CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME])[0])
    {
         
        
        if (buffer[0] != 5 || buffer[1] != 2)
            return 0;
        
         
        username_len = strlen (CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME]));
        password_len = strlen (CONFIG_STRING(proxy->options[PROXY_OPTION_PASSWORD]));
        
         
        buffer[0] = 1;
        buffer[1] = (unsigned char) username_len;
        memcpy(buffer + 2, CONFIG_STRING(proxy->options[PROXY_OPTION_USERNAME]), username_len);
        buffer[2 + username_len] = (unsigned char) password_len;
        memcpy (buffer + 3 + username_len,
                CONFIG_STRING(proxy->options[PROXY_OPTION_PASSWORD]), password_len);
        
        send (sock, buffer, 3 + username_len + password_len, 0);
        
         
        if (recv (sock, buffer, 2, 0) != 2)
            return 0;
        
         
        if (buffer[1] != 0)
            return 0;
    }
    else
    {
         
        if (!((buffer[0] == 5) && (buffer[1] == 0)))
            return 0;
    }
    
     
    addr_len = strlen(address);
    addr_buffer_len = 4 + 1 + addr_len + 2;
    addr_buffer = malloc (addr_buffer_len * sizeof(*addr_buffer));
    if (!addr_buffer)
        return 0;
    addr_buffer[0] = 5;    
    addr_buffer[1] = 1;    
    addr_buffer[2] = 0;    
    addr_buffer[3] = 3;    
    addr_buffer[4] = (unsigned char) addr_len;
    memcpy (addr_buffer + 5, address, addr_len);  
    *((unsigned short *) (addr_buffer + 5 + addr_len)) = htons (port);  
    
    send (sock, addr_buffer, addr_buffer_len, 0);
    free (addr_buffer);
    
     
    if (recv (sock, buffer, 4, 0) != 4)
        return 0;
    
    if (!((buffer[0] == 5) && (buffer[1] == 0)))
        return 0;
    
     
    switch (buffer[3])
    {
        case 1:
             
            if (recv (sock, buffer, 6, 0) != 6)
                return 0;
            break;
        case 3:
             
             
            if (recv (sock, buffer, 1, 0) != 1)
                return 0;
            addr_len = buffer[0];
             
            if (recv (sock, buffer, addr_len + 2, 0) != (addr_len + 2))
                return 0;
            break;
        case 4:
             
            if (recv (sock, buffer, 18, 0) != 18)
                return 0;
            break;
        default:
            return 0;
    }
    
     
    return 1;
}
