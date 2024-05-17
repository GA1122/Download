irc_server_alloc (const char *name)
{
    struct t_irc_server *new_server;
    int i, length;
    char *option_name;
    
    if (irc_server_search (name))
        return NULL;
    
     
    new_server = malloc (sizeof (*new_server));
    if (!new_server)
    {
        weechat_printf (NULL,
                        _("%s%s: error when allocating new server"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME);
        return NULL;
    }
    
     
    new_server->prev_server = last_irc_server;
    new_server->next_server = NULL;
    if (irc_servers)
        last_irc_server->next_server = new_server;
    else
        irc_servers = new_server;
    last_irc_server = new_server;
    
     
    new_server->name = strdup (name);
    
     
    new_server->temp_server = 0;
    new_server->reloading_from_config = 0;
    new_server->reloaded_from_config = 0;
    new_server->addresses_count = 0;
    new_server->addresses_array = NULL;
    new_server->ports_array = NULL;
    new_server->index_current_address = 0;
    new_server->current_address = NULL;
    new_server->current_ip = NULL;
    new_server->current_port = 0;
    new_server->sock = -1;
    new_server->hook_connect = NULL;
    new_server->hook_fd = NULL;
    new_server->hook_timer_connection = NULL;
    new_server->hook_timer_sasl = NULL;
    new_server->is_connected = 0;
    new_server->ssl_connected = 0;
    new_server->unterminated_message = NULL;
    new_server->nicks_count = 0;
    new_server->nicks_array = NULL;
    new_server->nick_first_tried = 0;
    new_server->nick = NULL;
    new_server->nick_modes = NULL;
    new_server->isupport = NULL;
    new_server->prefix_modes = NULL;
    new_server->prefix_chars = NULL;
    new_server->reconnect_delay = 0;
    new_server->reconnect_start = 0;
    new_server->command_time = 0;
    new_server->reconnect_join = 0;
    new_server->disable_autojoin = 0;
    new_server->is_away = 0;
    new_server->away_message = NULL;
    new_server->away_time = 0;
    new_server->lag = 0;
    new_server->lag_check_time.tv_sec = 0;
    new_server->lag_check_time.tv_usec = 0;
    new_server->lag_next_check = time (NULL) +
        weechat_config_integer (irc_config_network_lag_check);
    new_server->lag_last_refresh = 0;
    new_server->cmd_list_regexp = NULL;
    new_server->last_user_message = 0;
    new_server->last_away_check = 0;
    for (i = 0; i < IRC_SERVER_NUM_OUTQUEUES_PRIO; i++)
    {
        new_server->outqueue[i] = NULL;
        new_server->last_outqueue[i] = NULL;
    }
    new_server->redirects = NULL;
    new_server->last_redirect = NULL;
    new_server->notify_list = NULL;
    new_server->last_notify = NULL;
    new_server->buffer = NULL;
    new_server->buffer_as_string = NULL;
    new_server->channels = NULL;
    new_server->last_channel = NULL;
    
     
    for (i = 0; i < IRC_SERVER_NUM_OPTIONS; i++)
    {
        length = strlen (new_server->name) + 1 +
            strlen (irc_server_option_string[i]) + 1;
        option_name = malloc (length);
        if (option_name)
        {
            snprintf (option_name, length, "%s.%s",
                      new_server->name,
                      irc_server_option_string[i]);
            new_server->options[i] =
                irc_config_server_new_option (irc_config_file,
                                              irc_config_section_server,
                                              i,
                                              option_name,
                                              NULL,
                                              NULL,
                                              1,
                                              &irc_config_server_change_cb,
                                              irc_server_option_string[i]);
            irc_config_server_change_cb (irc_server_option_string[i],
                                         new_server->options[i]);
            free (option_name);
        }
    }
    
    return new_server;
}
