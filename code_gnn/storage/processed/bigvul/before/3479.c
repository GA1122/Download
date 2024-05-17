irc_server_free_data (struct t_irc_server *server)
{
    int i;
    
    if (!server)
        return;
    
     
    for (i = 0; i < IRC_SERVER_NUM_OPTIONS; i++)
    {
        if (server->options[i])
            weechat_config_option_free (server->options[i]);
    }
    if (server->name)
        free (server->name);
    if (server->addresses_array)
        weechat_string_free_split (server->addresses_array);
    if (server->ports_array)
        free (server->ports_array);
    if (server->current_address)
        free (server->current_address);
    if (server->current_ip)
        free (server->current_ip);
    if (server->hook_connect)
        weechat_unhook (server->hook_connect);
    if (server->hook_fd)
        weechat_unhook (server->hook_fd);
    if (server->hook_timer_connection)
        weechat_unhook (server->hook_timer_connection);
    if (server->hook_timer_sasl)
        weechat_unhook (server->hook_timer_sasl);
    if (server->unterminated_message)
        free (server->unterminated_message);
    if (server->nicks_array)
        weechat_string_free_split (server->nicks_array);
    if (server->nick)
        free (server->nick);
    if (server->nick_modes)
        free (server->nick_modes);
    if (server->isupport)
        free (server->isupport);
    if (server->prefix_modes)
        free (server->prefix_modes);
    if (server->prefix_chars)
        free (server->prefix_chars);
    if (server->away_message)
        free (server->away_message);
    if (server->cmd_list_regexp)
    {
        regfree (server->cmd_list_regexp);
        free (server->cmd_list_regexp);
    }
    for (i = 0; i < IRC_SERVER_NUM_OUTQUEUES_PRIO; i++)
    {
        irc_server_outqueue_free_all (server, i);
    }
    irc_notify_free_all (server);
    irc_redirect_free_all (server);
    if (server->channels)
        irc_channel_free_all (server);
    if (server->buffer_as_string)
        free (server->buffer_as_string);
}
