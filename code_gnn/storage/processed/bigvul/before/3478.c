irc_server_free_all ()
{
     
    while (irc_servers)
    {
        irc_server_free (irc_servers);
    }
}
