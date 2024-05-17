irc_server_copy (struct t_irc_server *server, const char *new_name)
{
    struct t_irc_server *new_server;
    struct t_infolist *infolist;
    char *mask, *pos;
    const char *option_name;
    int length, index_option;
    
     
    if (irc_server_search (new_name))
        return NULL;
    
    new_server = irc_server_alloc (new_name);
    if (new_server)
    {
         
        length = 32 + strlen (server->name) + 1;
        mask = malloc (length);
        if (!mask)
            return 0;
        snprintf (mask, length, "irc.server.%s.*", server->name);
        infolist = weechat_infolist_get ("option", NULL, mask);
        free (mask);
        while (weechat_infolist_next (infolist))
        {
            if (!weechat_infolist_integer (infolist, "value_is_null"))
            {
                option_name = weechat_infolist_string (infolist, "option_name");
                pos = strrchr (option_name, '.');
                if (pos)
                {
                    index_option = irc_server_search_option (pos + 1);
                    if (index_option >= 0)
                    {
                        weechat_config_option_set (new_server->options[index_option],
                                                   weechat_infolist_string (infolist, "value"),
                                                   1);
                    }
                }
            }
        }
    }
    
    return new_server;
}