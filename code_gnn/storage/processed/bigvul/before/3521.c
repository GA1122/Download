irc_server_set_nicks (struct t_irc_server *server, const char *nicks)
{
     
    server->nicks_count = 0;
    if (server->nicks_array)
    {
        weechat_string_free_split (server->nicks_array);
        server->nicks_array = NULL;
    }
    
     
    server->nicks_array = weechat_string_split ((nicks) ? nicks : IRC_SERVER_DEFAULT_NICKS,
                                                ",", 0, 0,
                                                &server->nicks_count);
}
