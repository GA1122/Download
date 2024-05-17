irc_server_timer_cb (void *data, int remaining_calls)
{
    struct t_irc_server *ptr_server;
    struct t_irc_redirect *ptr_redirect, *ptr_next_redirect;
    time_t current_time;
    static struct timeval tv;
    int away_check;
    
     
    (void) data;
    (void) remaining_calls;
    
    current_time = time (NULL);
    
    for (ptr_server = irc_servers; ptr_server;
         ptr_server = ptr_server->next_server)
    {
         
        if ((!ptr_server->is_connected)
            && (ptr_server->reconnect_start > 0)
            && (current_time >= (ptr_server->reconnect_start + ptr_server->reconnect_delay)))
        {
            irc_server_reconnect (ptr_server);
        }
        else
        {
            if (ptr_server->is_connected)
            {
                 
                irc_server_outqueue_send (ptr_server);
                
                 
                if ((weechat_config_integer (irc_config_network_lag_check) > 0)
                    && (ptr_server->lag_check_time.tv_sec == 0)
                    && (current_time >= ptr_server->lag_next_check))
                {
                    irc_server_sendf (ptr_server, 0, NULL, "PING %s",
                                      (ptr_server->current_address) ?
                                      ptr_server->current_address : "weechat");
                    gettimeofday (&(ptr_server->lag_check_time), NULL);
                    ptr_server->lag = 0;
                    ptr_server->lag_last_refresh = 0;
                }
                else
                {
                     
                    away_check = IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_AWAY_CHECK);
                    if (away_check > 0)
                    {
                        if ((ptr_server->last_away_check == 0)
                            || (current_time >= ptr_server->last_away_check + (away_check * 60)))
                        {
                            irc_server_check_away (ptr_server);
                        }
                    }
                }
                
                 
                if ((ptr_server->command_time != 0)
                    && (current_time >= ptr_server->command_time +
                        IRC_SERVER_OPTION_INTEGER(ptr_server, IRC_SERVER_OPTION_COMMAND_DELAY)))
                {
                    irc_server_autojoin_channels (ptr_server);
                    ptr_server->command_time = 0;
                }
                
                 
                if (ptr_server->lag_check_time.tv_sec != 0)
                {
                    gettimeofday (&tv, NULL);
                    ptr_server->lag = (int) weechat_util_timeval_diff (&(ptr_server->lag_check_time),
                                                                       &tv);
                     
                    if (((ptr_server->lag_last_refresh == 0)
                         || (current_time >= ptr_server->lag_last_refresh + weechat_config_integer (irc_config_network_lag_refresh_interval)))
                        && (ptr_server->lag >= weechat_config_integer (irc_config_network_lag_min_show)))
                    {
                        ptr_server->lag_last_refresh = current_time;
                        weechat_bar_item_update ("lag");
                    }
                     
                    if ((weechat_config_integer (irc_config_network_lag_disconnect) > 0)
                        && (ptr_server->lag / 1000 > weechat_config_integer (irc_config_network_lag_disconnect) * 60))
                    {
                        weechat_printf (ptr_server->buffer,
                                        _("%s: lag is high, disconnecting "
                                          "from server..."),
                                        IRC_PLUGIN_NAME);
                        irc_server_disconnect (ptr_server, 0, 1);
                    }
                }
                
                 
                ptr_redirect = ptr_server->redirects;
                while (ptr_redirect)
                {
                    ptr_next_redirect = ptr_redirect->next_redirect;
                    
                    if ((ptr_redirect->start_time > 0)
                        && (ptr_redirect->start_time + ptr_redirect->timeout < current_time))
                    {
                        irc_redirect_stop (ptr_redirect, "timeout");
                    }
                    
                    ptr_redirect = ptr_next_redirect;
                }
            }
        }
    }
    
    return WEECHAT_RC_OK;
}
