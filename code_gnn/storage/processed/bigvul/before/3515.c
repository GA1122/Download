irc_server_sendf (struct t_irc_server *server, int flags, const char *tags,
                  const char *format, ...)
{
    va_list args;
    static char buffer[4096];
    char **items;
    int i, items_count;
    
    if (!server)
        return;
    
    va_start (args, format);
    vsnprintf (buffer, sizeof (buffer) - 1, format, args);
    va_end (args);

    items = weechat_string_split (buffer, "\n", 0, 0, &items_count);
    for (i = 0; i < items_count; i++)
    {
        if (!irc_server_send_one_msg (server, flags, items[i], tags))
            break;
    }
    if (items)
        weechat_string_free_split (items);
}
