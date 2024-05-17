irc_server_set_index_current_address (struct t_irc_server *server, int index)
{
    if (server->current_address)
    {
        free (server->current_address);
        server->current_address = NULL;
    }
    server->current_port = 0;
    
    if (index < server->addresses_count)
    {
        server->index_current_address = index;
        if (server->current_address)
            free (server->current_address);
        server->current_address = strdup (server->addresses_array[index]);
        server->current_port = server->ports_array[index];
    }
}
