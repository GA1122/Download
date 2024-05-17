irc_server_alloc_with_url (const char *irc_url)
{
    char *irc_url2, *pos_server, *pos_nick, *pos_password;
    char *pos_address, *pos_port, *pos_channel, *pos;
    char *server_address, *server_nicks, *server_autojoin;
    int ipv6, ssl, length;
    struct t_irc_server *ptr_server;
    
    irc_url2 = strdup (irc_url);
    if (!irc_url2)
        return 0;
    
    pos_server = NULL;
    pos_nick = NULL;
    pos_password = NULL;
    pos_address = NULL;
    pos_port = NULL;
    pos_channel = NULL;
    
    ipv6 = 0;
    ssl = 0;
    
    pos_server = strstr (irc_url2, "://");
    if (!pos_server)
    {
        free (irc_url2);
        return 0;
    }
    pos_server[0] = '\0';
    pos_server += 3;
    
    pos_channel = strstr (pos_server, "/");
    if (pos_channel)
    {
        pos_channel[0] = '\0';
        pos_channel++;
        while (pos_channel[0] == '/')
        {
            pos_channel++;
        }
    }
    
     
    if (weechat_strcasecmp (irc_url2, "irc6") == 0)
    {
        ipv6 = 1;
    }
    else if (weechat_strcasecmp (irc_url2, "ircs") == 0)
    {
        ssl = 1;
    }
    else if ((weechat_strcasecmp (irc_url2, "irc6s") == 0)
             || (weechat_strcasecmp (irc_url2, "ircs6") == 0))
    {
        ipv6 = 1;
        ssl = 1;
    }
    
     
    pos_address = strchr (pos_server, '@');
    if (pos_address)
    {
        pos_address[0] = '\0';
        pos_address++;
        pos_nick = pos_server;
        pos_password = strchr (pos_server, ':');
        if (pos_password)
        {
            pos_password[0] = '\0';
            pos_password++;
        }
    }
    else
        pos_address = pos_server;
    
     
    if (pos_address[0] == '[')
    {
        pos_address++;
        pos = strchr (pos_address, ']');
        if (!pos)
        {
            free (irc_url2);
            return 0;
        }
        pos[0] = '\0';
        pos++;
        pos_port = strchr (pos, ':');
        if (pos_port)
        {
            pos_port[0] = '\0';
            pos_port++;
        }
    }
    else
    {
        pos_port = strchr (pos_address, ':');
        if (pos_port)
        {
            pos_port[0] = '\0';
            pos_port++;
        }
    }
    
    ptr_server = irc_server_alloc (pos_address);
    if (ptr_server)
    {
        ptr_server->temp_server = 1;
        if (pos_address && pos_address[0])
        {
            length = strlen (pos_address) + 1 +
                ((pos_port) ? strlen (pos_port) : 0) + 1;
            server_address = malloc (length);
            if (server_address)
            {
                snprintf (server_address, length,
                          "%s%s%s",
                          pos_address,
                          (pos_port && pos_port[0]) ? "/" : "",
                          (pos_port && pos_port[0]) ? pos_port : "");
                weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_ADDRESSES],
                                           server_address,
                                           1);
                free (server_address);
            }
        }
        weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_IPV6],
                                   (ipv6) ? "on" : "off",
                                   1);
        weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_SSL],
                                   (ssl) ? "on" : "off",
                                   1);
        if (pos_nick && pos_nick[0])
        {
            length = ((strlen (pos_nick) + 2) * 5) + 1;
            server_nicks = malloc (length);
            if (server_nicks)
            {
                snprintf (server_nicks, length,
                          "%s,%s1,%s2,%s3,%s4",
                          pos_nick, pos_nick, pos_nick, pos_nick, pos_nick);
                weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_NICKS],
                                           server_nicks,
                                           1);
                free (server_nicks);
            }
        }
        if (pos_password && pos_password[0])
            weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_PASSWORD],
                                       pos_password,
                                       1);
        weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_AUTOCONNECT],
                                   "on",
                                   1);
         
        if (pos_channel && pos_channel[0])
        {
            if (irc_channel_is_channel (pos_channel))
                server_autojoin = strdup (pos_channel);
            else
            {
                server_autojoin = malloc (strlen (pos_channel) + 2);
                if (server_autojoin)
                {
                    strcpy (server_autojoin, "#");
                    strcat (server_autojoin, pos_channel);
                }
            }
            if (server_autojoin)
            {
                weechat_config_option_set (ptr_server->options[IRC_SERVER_OPTION_AUTOJOIN],
                                           server_autojoin,
                                           1);
                free (server_autojoin);
            }
        }
    }
    else
    {
        weechat_printf (NULL,
                        _("%s%s: error creating new server \"%s\""),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME,
                        pos_address);
    }
    
    free (irc_url2);
    
    return (ptr_server) ? 1 : 0;
}
