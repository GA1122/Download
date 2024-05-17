irc_server_rename (struct t_irc_server *server, const char *new_server_name)
{
    int length;
    char *mask, *pos_option, *new_option_name, *buffer_name;
    char charset_modifier[256];
    const char *option_name;
    struct t_infolist *infolist;
    struct t_config_option *ptr_option;
    struct t_irc_channel *ptr_channel;
    
     
    if (irc_server_search (new_server_name))
        return 0;
    
     
    length = 32 + strlen (server->name) + 1;
    mask = malloc (length);
    if (!mask)
        return 0;
    snprintf (mask, length, "irc.server.%s.*", server->name);
    infolist = weechat_infolist_get ("option", NULL, mask);
    free (mask);
    while (weechat_infolist_next (infolist))
    {
        weechat_config_search_with_string (weechat_infolist_string (infolist,
                                                                    "full_name"),
                                           NULL, NULL, &ptr_option,
                                           NULL);
        if (ptr_option)
        {
            option_name = weechat_infolist_string (infolist, "option_name");
            if (option_name)
            {
                pos_option = strrchr (option_name, '.');
                if (pos_option)
                {
                    pos_option++;
                    length = strlen (new_server_name) + 1 + strlen (pos_option) + 1;
                    new_option_name = malloc (length);
                    if (new_option_name)
                    {
                        snprintf (new_option_name, length,
                                  "%s.%s", new_server_name, pos_option);
                        weechat_config_option_rename (ptr_option, new_option_name);
                        free (new_option_name);
                    }
                }
            }
        }
    }
    weechat_infolist_free (infolist);
    
     
    if (server->name)
        free (server->name);
    server->name = strdup (new_server_name);
    
     
    for (ptr_channel = server->channels; ptr_channel;
         ptr_channel = ptr_channel->next_channel)
    {
        if (ptr_channel->buffer)
        {
            buffer_name = irc_buffer_build_name (server->name, ptr_channel->name);
            weechat_buffer_set (ptr_channel->buffer, "name", buffer_name);
            weechat_buffer_set (ptr_channel->buffer, "localvar_set_server",
                                server->name);
        }
    }
    if (server->buffer)
    {
        buffer_name = irc_buffer_build_name (server->name, NULL);
        weechat_buffer_set (server->buffer, "name", buffer_name);
        weechat_buffer_set (server->buffer, "localvar_set_server",
                            server->name);
        weechat_buffer_set (server->buffer, "localvar_set_channel",
                            server->name);
        snprintf (charset_modifier, sizeof (charset_modifier),
                  "irc.%s", server->name);
        weechat_buffer_set (server->buffer, "localvar_set_charset_modifier",
                            charset_modifier);
    }
    
    return 1;
}
