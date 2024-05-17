irc_server_connect (struct t_irc_server *server)
{
    int set, length;
    char *option_name;
    struct t_config_option *proxy_type, *proxy_ipv6, *proxy_address, *proxy_port;
    const char *proxy, *str_proxy_type, *str_proxy_address;
    
    if (!server->buffer)
    {
        if (!irc_server_create_buffer (server))
            return 0;
        weechat_buffer_set (server->buffer, "display", "auto");
    }
    
    weechat_bar_item_update ("buffer_name");
    
    irc_server_set_index_current_address (server,
                                          server->index_current_address);
    
    if (!server->current_address)
    {
        weechat_printf (server->buffer,
                        _("%s%s: unknown address server \"%s\", "
                          "cannot connect"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME,
                        server->name);
        return 0;
    }
    
     
    if (server->isupport)
    {
        free (server->isupport);
        server->isupport = NULL;
    }
    if (server->prefix_modes)
    {
        free (server->prefix_modes);
        server->prefix_modes = NULL;
    }
    if (server->prefix_chars)
    {
        free (server->prefix_chars);
        server->prefix_chars = NULL;
    }
    
    proxy_type = NULL;
    proxy_ipv6 = NULL;
    proxy_address = NULL;
    proxy_port = NULL;
    str_proxy_type = NULL;
    str_proxy_address = NULL;
    
    proxy = IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_PROXY);
    if (proxy && proxy[0])
    {
        length = 32 + strlen (proxy) + 1;
        option_name = malloc (length);
        if (!option_name)
        {
            weechat_printf (server->buffer,
                            _("%s%s: not enough memory"),
                            weechat_prefix ("error"), IRC_PLUGIN_NAME);
            return 0;
        }
        snprintf (option_name, length, "weechat.proxy.%s.type", proxy);
        proxy_type = weechat_config_get (option_name);
        snprintf (option_name, length, "weechat.proxy.%s.ipv6", proxy);
        proxy_ipv6 = weechat_config_get (option_name);
        snprintf (option_name, length, "weechat.proxy.%s.address", proxy);
        proxy_address = weechat_config_get (option_name);
        snprintf (option_name, length, "weechat.proxy.%s.port", proxy);
        proxy_port = weechat_config_get (option_name);
        free (option_name);
        if (!proxy_type || !proxy_address)
        {
            weechat_printf (server->buffer,
                            _("%s%s: proxy \"%s\" not found for server "
                              "\"%s\", cannot connect"),
                            weechat_prefix ("error"), IRC_PLUGIN_NAME, proxy,
                            server->name);
            return 0;
        }
        str_proxy_type = weechat_config_string (proxy_type);
        str_proxy_address = weechat_config_string (proxy_address);
        if (!str_proxy_type[0] || !proxy_ipv6 || !str_proxy_address[0]
            || !proxy_port)
        {
            weechat_printf (server->buffer,
                            _("%s%s: missing proxy settings, check options "
                              "for proxy \"%s\""),
                            weechat_prefix ("error"), IRC_PLUGIN_NAME, proxy);
            return 0;
        }
    }
    
    if (!server->nicks_array)
    {
        weechat_printf (server->buffer,
                        _("%s%s: nicks not defined for server \"%s\", "
                          "cannot connect"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME,
                        server->name);
        return 0;
    }
    
#ifndef HAVE_GNUTLS
    if (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_SSL))
    {
        weechat_printf (server->buffer,
                        _("%s%s: cannot connect with SSL because WeeChat "
                          "was not built with GnuTLS support"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME);
        return 0;
    }
#endif
    if (proxy_type)
    {
        weechat_printf (server->buffer,
                        _("%s: connecting to server %s/%d%s%s via %s "
                          "proxy %s/%d%s..."),
                        IRC_PLUGIN_NAME,
                        server->current_address,
                        server->current_port,
                        (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6)) ?
                        " (IPv6)" : "",
                        (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_SSL)) ?
                        " (SSL)" : "",
                        str_proxy_type,
                        str_proxy_address,
                        weechat_config_integer (proxy_port),
                        (weechat_config_boolean (proxy_ipv6)) ? " (IPv6)" : "");
        weechat_log_printf (_("Connecting to server %s/%d%s%s via %s proxy "
                              "%s/%d%s..."),
                            server->current_address,
                            server->current_port,
                            (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6)) ?
                            " (IPv6)" : "",
                            (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_SSL)) ?
                            " (SSL)" : "",
                            str_proxy_type,
                            str_proxy_address,
                            weechat_config_integer (proxy_port),
                            (weechat_config_boolean (proxy_ipv6)) ? " (IPv6)" : "");
    }
    else
    {
        weechat_printf (server->buffer,
                        _("%s: connecting to server %s/%d%s%s..."),
                        IRC_PLUGIN_NAME,
                        server->current_address,
                        server->current_port,
                        (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6)) ?
                        " (IPv6)" : "",
                        (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_SSL)) ?
                        " (SSL)" : "");
        weechat_log_printf (_("%s%s: connecting to server %s/%d%s%s..."),
                            "",
                            IRC_PLUGIN_NAME,
                            server->current_address,
                            server->current_port,
                            (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6)) ?
                            " (IPv6)" : "",
                            (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_SSL)) ?
                            " (SSL)" : "");
    }
    
     
    irc_server_close_connection (server);
    
     
    if (proxy_type)
    {
        server->sock = socket ((weechat_config_integer (proxy_ipv6)) ?
                               AF_INET6 : AF_INET,
                               SOCK_STREAM, 0);
    }
    else
    {
        server->sock = socket ((IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6)) ?
                               AF_INET6 : AF_INET,
                               SOCK_STREAM, 0);
    }
    if (server->sock == -1)
    {
        weechat_printf (server->buffer,
                        _("%s%s: cannot create socket"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME);
        return 0;
    }
    
     
    set = 1;
    if (setsockopt (server->sock, SOL_SOCKET, SO_REUSEADDR,
        (void *) &set, sizeof (set)) == -1)
    {
        weechat_printf (server->buffer,
                        _("%s%s: cannot set socket option "
                          "\"SO_REUSEADDR\""),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME);
    }
    
     
    set = 1;
    if (setsockopt (server->sock, SOL_SOCKET, SO_KEEPALIVE,
        (void *) &set, sizeof (set)) == -1)
    {
        weechat_printf (server->buffer,
                        _("%s%s: cannot set socket option "
                          "\"SO_KEEPALIVE\""),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME);
    }
    
     
    server->ssl_connected = 0;
#ifdef HAVE_GNUTLS
    if (IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_SSL))
        server->ssl_connected = 1;
    server->hook_connect = weechat_hook_connect (proxy,
                                                 server->current_address,
                                                 server->current_port,
                                                 server->sock,
                                                 IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6),
                                                 (server->ssl_connected) ? &server->gnutls_sess : NULL,
                                                 (server->ssl_connected) ? irc_server_gnutls_callback : NULL,
                                                 IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_SSL_DHKEY_SIZE),
                                                 IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_LOCAL_HOSTNAME),
                                                 &irc_server_connect_cb,
                                                 server);
#else
    server->hook_connect = weechat_hook_connect (proxy,
                                                 server->current_address,
                                                 server->current_port,
                                                 server->sock,
                                                 IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_IPV6),
                                                 NULL, NULL, 0,
                                                 IRC_SERVER_OPTION_STRING(server, IRC_SERVER_OPTION_LOCAL_HOSTNAME),
                                                 &irc_server_connect_cb,
                                                 server);
#endif
    
     
    weechat_hook_signal_send ("irc_server_connecting",
                              WEECHAT_HOOK_SIGNAL_STRING, server->name);
    
    return 1;
}
