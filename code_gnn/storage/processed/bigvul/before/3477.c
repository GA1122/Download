irc_server_free (struct t_irc_server *server)
{
    struct t_irc_server *new_irc_servers;
    
    if (!server)
        return;
    
     
    irc_channel_free_all (server);
    
     
    if (last_irc_server == server)
        last_irc_server = server->prev_server;
    if (server->prev_server)
    {
        (server->prev_server)->next_server = server->next_server;
        new_irc_servers = irc_servers;
    }
    else
        new_irc_servers = server->next_server;
    
    if (server->next_server)
        (server->next_server)->prev_server = server->prev_server;
    
    irc_server_free_data (server);
    free (server);
    irc_servers = new_irc_servers;
}
