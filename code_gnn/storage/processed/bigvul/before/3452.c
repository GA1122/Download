network_connect_child (struct t_hook *hook_connect)
{
    struct t_proxy *ptr_proxy;
    struct addrinfo hints, *res, *res_local, *ptr_res;
    char status_str[2], *ptr_address, *status_ok_with_address;
    char ipv4_address[INET_ADDRSTRLEN + 1], ipv6_address[INET6_ADDRSTRLEN + 1];
    char status_ok_without_address[1 + 5 + 1];
    int rc, length;
    
    res = NULL;
    res_local = NULL;
    
    status_str[1] = '\0';
    
    ptr_proxy = NULL;
    if (HOOK_CONNECT(hook_connect, proxy)
        && HOOK_CONNECT(hook_connect, proxy)[0])
    {
        ptr_proxy = proxy_search (HOOK_CONNECT(hook_connect, proxy));
        if (!ptr_proxy)
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_PROXY_ERROR;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            return;
        }
    }
    
    if (ptr_proxy)
    {
         
        memset (&hints, 0, sizeof (hints));
        hints.ai_family = (CONFIG_BOOLEAN(ptr_proxy->options[PROXY_OPTION_IPV6])) ?
            AF_INET6 : AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if (getaddrinfo (CONFIG_STRING(ptr_proxy->options[PROXY_OPTION_ADDRESS]), NULL, &hints, &res) !=0)
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_ADDRESS_NOT_FOUND;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            return;
        }
        if (!res)
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_ADDRESS_NOT_FOUND;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            return;
        }
        if ((CONFIG_BOOLEAN(ptr_proxy->options[PROXY_OPTION_IPV6]) && (res->ai_family != AF_INET6))
            || ((!CONFIG_BOOLEAN(ptr_proxy->options[PROXY_OPTION_IPV6]) && (res->ai_family != AF_INET))))
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_IP_ADDRESS_NOT_FOUND;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            freeaddrinfo (res);
            return;
        }
        
        if (CONFIG_BOOLEAN(ptr_proxy->options[PROXY_OPTION_IPV6]))
            ((struct sockaddr_in6 *)(res->ai_addr))->sin6_port = htons (CONFIG_INTEGER(ptr_proxy->options[PROXY_OPTION_PORT]));
        else
            ((struct sockaddr_in *)(res->ai_addr))->sin_port = htons (CONFIG_INTEGER(ptr_proxy->options[PROXY_OPTION_PORT]));
        
         
        if (connect (HOOK_CONNECT(hook_connect, sock),
                     res->ai_addr, res->ai_addrlen) != 0)
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_CONNECTION_REFUSED;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            freeaddrinfo (res);
            return;
        }
        
        if (!network_pass_proxy (HOOK_CONNECT(hook_connect, proxy),
                                 HOOK_CONNECT(hook_connect, sock),
                                 HOOK_CONNECT(hook_connect, address),
                                 HOOK_CONNECT(hook_connect, port)))
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_PROXY_ERROR;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            freeaddrinfo (res);
            return;
        }

        status_str[0] = '0' + WEECHAT_HOOK_CONNECT_OK;
    }
    else
    {
         
        if (HOOK_CONNECT(hook_connect, local_hostname)
            && HOOK_CONNECT(hook_connect, local_hostname[0]))
        {
            memset (&hints, 0, sizeof(hints));
            hints.ai_family = (HOOK_CONNECT(hook_connect, ipv6)) ? AF_INET6 : AF_INET;
            hints.ai_socktype = SOCK_STREAM;
            rc = getaddrinfo (HOOK_CONNECT(hook_connect, local_hostname),
                              NULL, &hints, &res_local);
            if ((rc != 0) || !res_local
                || (HOOK_CONNECT(hook_connect, ipv6)
                    && (res_local->ai_family != AF_INET6))
                || ((!HOOK_CONNECT(hook_connect, ipv6)
                     && (res_local->ai_family != AF_INET))))
            {
                 
                status_str[0] = '0' + WEECHAT_HOOK_CONNECT_LOCAL_HOSTNAME_ERROR;
                write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
                if (res_local)
                    freeaddrinfo (res_local);
                return;
            }
            if (bind (HOOK_CONNECT(hook_connect, sock),
                      res_local->ai_addr, res_local->ai_addrlen) < 0)
            {
                 
                status_str[0] = '0' + WEECHAT_HOOK_CONNECT_LOCAL_HOSTNAME_ERROR;
                write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
                if (res_local)
                    freeaddrinfo (res_local);
                return;
            }
        }
        
         
        memset (&hints, 0, sizeof(hints));
        hints.ai_family = (HOOK_CONNECT(hook_connect, ipv6)) ? AF_INET6 : AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        rc = getaddrinfo (HOOK_CONNECT(hook_connect, address),
                          NULL, &hints, &res);
        if ((rc != 0) || !res)
        {
             
            status_str[0] = '0' + WEECHAT_HOOK_CONNECT_ADDRESS_NOT_FOUND;
            write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
            if (res)
                freeaddrinfo (res);
            if (res_local)
                freeaddrinfo (res_local);
            return;
        }
        
        status_str[0] = '0' + WEECHAT_HOOK_CONNECT_IP_ADDRESS_NOT_FOUND;
        
         
        for (ptr_res = res; ptr_res; ptr_res = ptr_res->ai_next)
        {
             
            if ((HOOK_CONNECT(hook_connect, ipv6) && (ptr_res->ai_family != AF_INET6))
                || ((!HOOK_CONNECT(hook_connect, ipv6) && (ptr_res->ai_family != AF_INET))))
                continue;
            
             
            if (HOOK_CONNECT(hook_connect, ipv6))
                ((struct sockaddr_in6 *)(ptr_res->ai_addr))->sin6_port =
                    htons (HOOK_CONNECT(hook_connect, port));
            else
                ((struct sockaddr_in *)(ptr_res->ai_addr))->sin_port =
                    htons (HOOK_CONNECT(hook_connect, port));
            
            if (connect (HOOK_CONNECT(hook_connect, sock),
                         ptr_res->ai_addr, ptr_res->ai_addrlen) == 0)
            {
                status_str[0] = '0' + WEECHAT_HOOK_CONNECT_OK;
                break;
            }
            else
                status_str[0] = '0' + WEECHAT_HOOK_CONNECT_CONNECTION_REFUSED;
        }
    }
    
    if (status_str[0] == '0' + WEECHAT_HOOK_CONNECT_OK)
    {
        status_ok_with_address = NULL;
        ptr_address = NULL;
        if (HOOK_CONNECT(hook_connect, ipv6))
        {
            if (inet_ntop (AF_INET6,
                           &((struct sockaddr_in6 *)(res->ai_addr))->sin6_addr,
                           ipv6_address,
                           INET6_ADDRSTRLEN))
            {
                ptr_address = ipv6_address;
            }
        }
        else
        {
            if (inet_ntop (AF_INET,
                           &((struct sockaddr_in *)(res->ai_addr))->sin_addr,
                           ipv4_address,
                           INET_ADDRSTRLEN))
            {
                ptr_address = ipv4_address;
            }
        }
        if (ptr_address)
        {
            length = strlen (status_str) + 5 + strlen (ptr_address) + 1;
            status_ok_with_address = malloc (length);
            if (status_ok_with_address)
            {
                snprintf (status_ok_with_address, length, "%s%05d%s",
                          status_str, (int)strlen (ptr_address), ptr_address);
            }
        }
        
        if (status_ok_with_address)
        {
            write (HOOK_CONNECT(hook_connect, child_write),
                   status_ok_with_address, strlen (status_ok_with_address));
            free (status_ok_with_address);
        }
        else
        {
            snprintf (status_ok_without_address, sizeof (status_ok_without_address),
                      "%s%05d", status_str, 0);
            write (HOOK_CONNECT(hook_connect, child_write),
                   status_ok_without_address, strlen (status_ok_without_address));
        }
    }
    else
    {
        write (HOOK_CONNECT(hook_connect, child_write), status_str, 1);
    }
    
    if (res)
        freeaddrinfo (res);
    if (res_local)
        freeaddrinfo (res_local);
}
