irc_server_print_log ()
{
    struct t_irc_server *ptr_server;
    struct t_irc_channel *ptr_channel;
    int i;
    
    for (ptr_server = irc_servers; ptr_server;
         ptr_server = ptr_server->next_server)
    {
        weechat_log_printf ("");
        weechat_log_printf ("[server %s (addr:0x%lx)]", ptr_server->name, ptr_server);
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_ADDRESSES]))
            weechat_log_printf ("  addresses. . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_ADDRESSES));
        else
            weechat_log_printf ("  addresses. . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_ADDRESSES]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_PROXY]))
            weechat_log_printf ("  proxy. . . . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_PROXY));
        else
            weechat_log_printf ("  proxy. . . . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_PROXY]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_IPV6]))
            weechat_log_printf ("  ipv6 . . . . . . . . : null (%s)",
                                (IRC_SERVER_OPTION_BOOLEAN(ptr_server, IRC_SERVER_OPTION_IPV6)) ?
                                "on" : "off");
        else
            weechat_log_printf ("  ipv6 . . . . . . . . : %s",
                                weechat_config_boolean (ptr_server->options[IRC_SERVER_OPTION_IPV6]) ?
                                "on" : "off");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SSL]))
            weechat_log_printf ("  ssl. . . . . . . . . : null (%s)",
                                (IRC_SERVER_OPTION_BOOLEAN(ptr_server, IRC_SERVER_OPTION_SSL)) ?
                                "on" : "off");
        else
            weechat_log_printf ("  ssl. . . . . . . . . : %s",
                                weechat_config_boolean (ptr_server->options[IRC_SERVER_OPTION_SSL]) ?
                                "on" : "off");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SSL_CERT]))
            weechat_log_printf ("  ssl_cert . . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_SSL_CERT));
        else
            weechat_log_printf ("  ssl_cert . . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_SSL_CERT]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SSL_DHKEY_SIZE]))
            weechat_log_printf ("  ssl_dhkey_size . . . : null ('%d')",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_SSL_DHKEY_SIZE));
        else
            weechat_log_printf ("  ssl_dhkey_size . . . : '%d'",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_SSL_DHKEY_SIZE]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SSL_VERIFY]))
            weechat_log_printf ("  ssl_verify . . . . . : null (%s)",
                                (IRC_SERVER_OPTION_BOOLEAN(ptr_server, IRC_SERVER_OPTION_SSL_VERIFY)) ?
                                "on" : "off");
        else
            weechat_log_printf ("  ssl_verify . . . . . : %s",
                                weechat_config_boolean (ptr_server->options[IRC_SERVER_OPTION_SSL_VERIFY]) ?
                                "on" : "off");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_PASSWORD]))
            weechat_log_printf ("  password . . . . . . : null");
        else
            weechat_log_printf ("  password . . . . . . : (hidden)");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SASL_MECHANISM]))
            weechat_log_printf ("  sasl_mechanism . . . : null ('%s')",
                                irc_sasl_mechanism_string[IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_SASL_MECHANISM)]);
        else
            weechat_log_printf ("  sasl_mechanism . . . : '%s'",
                                irc_sasl_mechanism_string[weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_SASL_MECHANISM])]);
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SASL_USERNAME]))
            weechat_log_printf ("  sasl_username. . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_SASL_USERNAME));
        else
            weechat_log_printf ("  sasl_username. . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_USERNAME]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_SASL_PASSWORD]))
            weechat_log_printf ("  sasl_password. . . . : null");
        else
            weechat_log_printf ("  sasl_password. . . . : (hidden)");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AUTOCONNECT]))
            weechat_log_printf ("  autoconnect. . . . . : null (%s)",
                                (IRC_SERVER_OPTION_BOOLEAN(ptr_server, IRC_SERVER_OPTION_AUTOCONNECT)) ?
                                "on" : "off");
        else
            weechat_log_printf ("  autoconnect. . . . . : %s",
                                weechat_config_boolean (ptr_server->options[IRC_SERVER_OPTION_AUTOCONNECT]) ?
                                "on" : "off");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AUTORECONNECT]))
            weechat_log_printf ("  autoreconnect. . . . : null (%s)",
                                (IRC_SERVER_OPTION_BOOLEAN(ptr_server, IRC_SERVER_OPTION_AUTORECONNECT)) ?
                                "on" : "off");
        else
            weechat_log_printf ("  autoreconnect. . . . : %s",
                                weechat_config_boolean (ptr_server->options[IRC_SERVER_OPTION_AUTORECONNECT]) ?
                                "on" : "off");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AUTORECONNECT_DELAY]))
            weechat_log_printf ("  autoreconnect_delay. : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_AUTORECONNECT_DELAY));
        else
            weechat_log_printf ("  autoreconnect_delay. : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_AUTORECONNECT_DELAY]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_NICKS]))
            weechat_log_printf ("  nicks. . . . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_NICKS));
        else
            weechat_log_printf ("  nicks. . . . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_NICKS]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_USERNAME]))
            weechat_log_printf ("  username . . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_USERNAME));
        else
            weechat_log_printf ("  username . . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_USERNAME]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_REALNAME]))
            weechat_log_printf ("  realname . . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_REALNAME));
        else
            weechat_log_printf ("  realname . . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_REALNAME]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_LOCAL_HOSTNAME]))
            weechat_log_printf ("  local_hostname . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_LOCAL_HOSTNAME));
        else
            weechat_log_printf ("  local_hostname . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_LOCAL_HOSTNAME]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_COMMAND]))
            weechat_log_printf ("  command. . . . . . . : null");
        else
            weechat_log_printf ("  command. . . . . . . : (hidden)");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_COMMAND_DELAY]))
            weechat_log_printf ("  command_delay. . . . : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_COMMAND_DELAY));
        else
            weechat_log_printf ("  command_delay. . . . : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_COMMAND_DELAY]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AUTOJOIN]))
            weechat_log_printf ("  autojoin . . . . . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_AUTOJOIN));
        else
            weechat_log_printf ("  autojoin . . . . . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_AUTOJOIN]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AUTOREJOIN]))
            weechat_log_printf ("  autorejoin . . . . . : null (%s)",
                                (IRC_SERVER_OPTION_BOOLEAN(ptr_server, IRC_SERVER_OPTION_AUTOREJOIN)) ?
                                "on" : "off");
        else
            weechat_log_printf ("  autorejoin . . . . . : %s",
                                weechat_config_boolean (ptr_server->options[IRC_SERVER_OPTION_AUTOREJOIN]) ?
                                "on" : "off");
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AUTOREJOIN_DELAY]))
            weechat_log_printf ("  autorejoin_delay . . : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_AUTOREJOIN_DELAY));
        else
            weechat_log_printf ("  autorejoin_delay . . : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_AUTOREJOIN_DELAY]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_CONNECTION_TIMEOUT]))
            weechat_log_printf ("  connection_timeout . : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_CONNECTION_TIMEOUT));
        else
            weechat_log_printf ("  connection_timeout . : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_CONNECTION_TIMEOUT]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_HIGH]))
            weechat_log_printf ("  anti_flood_prio_high : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_HIGH));
        else
            weechat_log_printf ("  anti_flood_prio_high : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_HIGH]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_LOW]))
            weechat_log_printf ("  anti_flood_prio_low. : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_LOW));
        else
            weechat_log_printf ("  anti_flood_prio_low. : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_LOW]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AWAY_CHECK]))
            weechat_log_printf ("  away_check . . . . . : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_AWAY_CHECK));
        else
            weechat_log_printf ("  away_check . . . . . : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_AWAY_CHECK]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_AWAY_CHECK_MAX_NICKS]))
            weechat_log_printf ("  away_check_max_nicks : null (%d)",
                                IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_AWAY_CHECK_MAX_NICKS));
        else
            weechat_log_printf ("  away_check_max_nicks : %d",
                                weechat_config_integer (ptr_server->options[IRC_SERVER_OPTION_AWAY_CHECK_MAX_NICKS]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_DEFAULT_MSG_PART]))
            weechat_log_printf ("  default_msg_part . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_DEFAULT_MSG_PART));
        else
            weechat_log_printf ("  default_msg_part . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_DEFAULT_MSG_PART]));
         
        if (weechat_config_option_is_null (ptr_server->options[IRC_SERVER_OPTION_DEFAULT_MSG_QUIT]))
            weechat_log_printf ("  default_msg_quit . . : null ('%s')",
                                IRC_SERVER_OPTION_STRING(ptr_server, IRC_SERVER_OPTION_DEFAULT_MSG_QUIT));
        else
            weechat_log_printf ("  default_msg_quit . . : '%s'",
                                weechat_config_string (ptr_server->options[IRC_SERVER_OPTION_DEFAULT_MSG_QUIT]));
         
        weechat_log_printf ("  temp_server. . . . . : %d",    ptr_server->temp_server);
        weechat_log_printf ("  reloading_from_config: %d",    ptr_server->reloaded_from_config);
        weechat_log_printf ("  reloaded_from_config : %d",    ptr_server->reloaded_from_config);
        weechat_log_printf ("  addresses_count. . . : %d",    ptr_server->addresses_count);
        weechat_log_printf ("  addresses_array. . . : 0x%lx", ptr_server->addresses_array);
        weechat_log_printf ("  ports_array. . . . . : 0x%lx", ptr_server->ports_array);
        weechat_log_printf ("  index_current_address: %d",    ptr_server->index_current_address);
        weechat_log_printf ("  current_address. . . : '%s'",  ptr_server->current_address);
        weechat_log_printf ("  current_ip . . . . . : '%s'",  ptr_server->current_ip);
        weechat_log_printf ("  current_port . . . . : %d",    ptr_server->current_port);
        weechat_log_printf ("  sock . . . . . . . . : %d",    ptr_server->sock);
        weechat_log_printf ("  hook_connect . . . . : 0x%lx", ptr_server->hook_connect);
        weechat_log_printf ("  hook_fd. . . . . . . : 0x%lx", ptr_server->hook_fd);
        weechat_log_printf ("  hook_timer_connection: 0x%lx", ptr_server->hook_timer_connection);
        weechat_log_printf ("  hook_timer_sasl. . . : 0x%lx", ptr_server->hook_timer_sasl);
        weechat_log_printf ("  is_connected . . . . : %d",    ptr_server->is_connected);
        weechat_log_printf ("  ssl_connected. . . . : %d",    ptr_server->ssl_connected);
#ifdef HAVE_GNUTLS
        weechat_log_printf ("  gnutls_sess. . . . . : 0x%lx", ptr_server->gnutls_sess);
#endif
        weechat_log_printf ("  unterminated_message : '%s'",  ptr_server->unterminated_message);
        weechat_log_printf ("  nicks_count. . . . . : %d",    ptr_server->nicks_count);
        weechat_log_printf ("  nicks_array. . . . . : 0x%lx", ptr_server->nicks_array);
        weechat_log_printf ("  nick_first_tried . . : %d",    ptr_server->nick_first_tried);
        weechat_log_printf ("  nick . . . . . . . . : '%s'",  ptr_server->nick);
        weechat_log_printf ("  nick_modes . . . . . : '%s'",  ptr_server->nick_modes);
        weechat_log_printf ("  isupport . . . . . . : '%s'",  ptr_server->isupport);
        weechat_log_printf ("  prefix_modes . . . . : '%s'",  ptr_server->prefix_modes);
        weechat_log_printf ("  prefix_chars . . . . : '%s'",  ptr_server->prefix_chars);
        weechat_log_printf ("  reconnect_delay. . . : %d",    ptr_server->reconnect_delay);
        weechat_log_printf ("  reconnect_start. . . : %ld",   ptr_server->reconnect_start);
        weechat_log_printf ("  command_time . . . . : %ld",   ptr_server->command_time);
        weechat_log_printf ("  reconnect_join . . . : %d",    ptr_server->reconnect_join);
        weechat_log_printf ("  disable_autojoin . . : %d",    ptr_server->disable_autojoin);
        weechat_log_printf ("  is_away. . . . . . . : %d",    ptr_server->is_away);
        weechat_log_printf ("  away_message . . . . : '%s'",  ptr_server->away_message);
        weechat_log_printf ("  away_time. . . . . . : %ld",   ptr_server->away_time);
        weechat_log_printf ("  lag. . . . . . . . . : %d",    ptr_server->lag);
        weechat_log_printf ("  lag_check_time . . . : tv_sec:%d, tv_usec:%d",
                            ptr_server->lag_check_time.tv_sec,
                            ptr_server->lag_check_time.tv_usec);
        weechat_log_printf ("  lag_next_check . . . : %ld",   ptr_server->lag_next_check);
        weechat_log_printf ("  lag_last_refresh . . : %ld",   ptr_server->lag_last_refresh);
        weechat_log_printf ("  cmd_list_regexp. . . : 0x%lx", ptr_server->cmd_list_regexp);
        weechat_log_printf ("  last_user_message. . : %ld",   ptr_server->last_user_message);
        weechat_log_printf ("  last_away_check. . . : %ld",   ptr_server->last_away_check);
        for (i = 0; i < IRC_SERVER_NUM_OUTQUEUES_PRIO; i++)
        {
            weechat_log_printf ("  outqueue[%02d] . . . . : 0x%lx", i, ptr_server->outqueue[i]);
            weechat_log_printf ("  last_outqueue[%02d]. . : 0x%lx", i, ptr_server->last_outqueue[i]);
        }
        weechat_log_printf ("  redirects. . . . . . : 0x%lx", ptr_server->redirects);
        weechat_log_printf ("  last_redirect. . . . : 0x%lx", ptr_server->last_redirect);
        weechat_log_printf ("  notify_list. . . . . : 0x%lx", ptr_server->notify_list);
        weechat_log_printf ("  last_notify. . . . . : 0x%lx", ptr_server->last_notify);
        weechat_log_printf ("  buffer . . . . . . . : 0x%lx", ptr_server->buffer);
        weechat_log_printf ("  buffer_as_string . . : 0x%lx", ptr_server->buffer_as_string);
        weechat_log_printf ("  channels . . . . . . : 0x%lx", ptr_server->channels);
        weechat_log_printf ("  last_channel . . . . : 0x%lx", ptr_server->last_channel);
        weechat_log_printf ("  prev_server. . . . . : 0x%lx", ptr_server->prev_server);
        weechat_log_printf ("  next_server. . . . . : 0x%lx", ptr_server->next_server);
        
        irc_redirect_print_log (ptr_server);
        
        irc_notify_print_log (ptr_server);
        
        for (ptr_channel = ptr_server->channels; ptr_channel;
             ptr_channel = ptr_channel->next_channel)
        {
            irc_channel_print_log (ptr_channel);
        }
    }
}
