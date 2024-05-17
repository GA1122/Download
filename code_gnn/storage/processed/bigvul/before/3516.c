irc_server_set_addresses (struct t_irc_server *server, const char *addresses)
{
    int i;
    char *pos, *error;
    long number;
    
     
    server->addresses_count = 0;
    if (server->addresses_array)
    {
        weechat_string_free_split (server->addresses_array);
        server->addresses_array = NULL;
    }
    if (server->ports_array)
    {
        free (server->ports_array);
        server->ports_array = NULL;
    }
    
     
    if (addresses && addresses[0])
    {
        server->addresses_array = weechat_string_split (addresses,
                                                        ",", 0, 0,
                                                        &server->addresses_count);
        server->ports_array = malloc (server->addresses_count * sizeof (server->ports_array[0]));
        for (i = 0; i < server->addresses_count; i++)
        {
            pos = strchr (server->addresses_array[i], '/');
            if (pos)
            {
                pos[0] = 0;
                pos++;
                error = NULL;
                number = strtol (pos, &error, 10);
                server->ports_array[i] = (error && !error[0]) ?
                    number : IRC_SERVER_DEFAULT_PORT;
            }
            else
            {
                server->ports_array[i] = IRC_SERVER_DEFAULT_PORT;
            }
        }
    }
}
