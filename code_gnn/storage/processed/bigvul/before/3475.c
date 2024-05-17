irc_server_disconnect (struct t_irc_server *server, int switch_address,
                       int reconnect)
{
    struct t_irc_channel *ptr_channel;
    
    if (server->is_connected)
    {
         
        for (ptr_channel = server->channels; ptr_channel;
             ptr_channel = ptr_channel->next_channel)
        {
            irc_nick_free_all (server, ptr_channel);
            weechat_printf (ptr_channel->buffer,
                            _("%s%s: disconnected from server"),
                            "",
                            IRC_PLUGIN_NAME);
        }
    }
    
    irc_server_close_connection (server);
    
    if (server->buffer)
    {
        weechat_printf (server->buffer,
                        _("%s: disconnected from server"),
                        IRC_PLUGIN_NAME);
    }
    
    if (switch_address)
        irc_server_switch_address (server, 0);
    else
        irc_server_set_index_current_address(server, 0);
    
    if (server->nick_modes)
    {
        free (server->nick_modes);
        server->nick_modes = NULL;
        weechat_bar_item_update ("input_prompt");
    }
    server->is_away = 0;
    server->away_time = 0;
    server->lag = 0;
    server->lag_check_time.tv_sec = 0;
    server->lag_check_time.tv_usec = 0;
    server->lag_next_check = time (NULL) +
        weechat_config_integer (irc_config_network_lag_check);
    server->lag_last_refresh = 0;
    
    if (reconnect
        && IRC_SERVER_OPTION_BOOLEAN(server, IRC_SERVER_OPTION_AUTORECONNECT))
        irc_server_reconnect_schedule (server);
    else
    {
        server->reconnect_delay = 0;
        server->reconnect_start = 0;
    }
    
     
    if (!reconnect && server->nick)
        irc_server_set_nick (server, NULL);
    
    irc_server_set_buffer_title (server);
    
     
    weechat_hook_signal_send ("irc_server_disconnected",
                              WEECHAT_HOOK_SIGNAL_STRING, server->name);
}
