irc_server_send_one_msg (struct t_irc_server *server, int flags,
                         const char *message, const char *tags)
{
    static char buffer[4096];
    const char *ptr_msg;
    char *new_msg, *pos, *nick, *command, *channel, *tags_to_send;
    char *ptr_chan_nick, *msg_encoded;
    char str_modifier[64], modifier_data[256];
    int rc, queue_msg, add_to_queue, first_message, anti_flood;
    time_t time_now;
    struct t_irc_redirect *ptr_redirect;
    
    rc = 1;
    
    irc_message_parse (message, &nick, NULL, &command, &channel, NULL);
    snprintf (str_modifier, sizeof (str_modifier),
              "irc_out_%s",
              (command) ? command : "unknown");
    new_msg = weechat_hook_modifier_exec (str_modifier,
                                          server->name,
                                          message);
    
     
    if (new_msg && (strcmp (message, new_msg) == 0))
    {
        free (new_msg);
        new_msg = NULL;
    }
    
     
    if (!new_msg || new_msg[0])
    {
        first_message = 1;
        ptr_msg = (new_msg) ? new_msg : message;

        msg_encoded = NULL;
        ptr_chan_nick = (channel) ? channel : nick;
        if (ptr_chan_nick)
        {
            snprintf (modifier_data, sizeof (modifier_data),
                      "%s.%s.%s",
                      weechat_plugin->name,
                      server->name,
                      ptr_chan_nick);
        }
        else
        {
            snprintf (modifier_data, sizeof (modifier_data),
                      "%s.%s",
                      weechat_plugin->name,
                      server->name);
        }
        msg_encoded = weechat_hook_modifier_exec ("charset_encode",
                                                  modifier_data,
                                                  ptr_msg);
        
        if (msg_encoded)
            ptr_msg = msg_encoded;
        
        while (rc && ptr_msg && ptr_msg[0])
        {
            pos = strchr (ptr_msg, '\n');
            if (pos)
                pos[0] = '\0';
            
            snprintf (buffer, sizeof (buffer) - 1, "%s\r\n", ptr_msg);

             
            time_now = time (NULL);
            
             
            if (server->last_user_message > time_now)
                server->last_user_message = time_now;
            
             
            queue_msg = 0;
            if (flags & IRC_SERVER_SEND_OUTQ_PRIO_HIGH)
                queue_msg = 1;
            else if (flags & IRC_SERVER_SEND_OUTQ_PRIO_LOW)
                queue_msg = 2;
            
            switch (queue_msg - 1)
            {
                case 0:
                    anti_flood = IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_HIGH);
                    break;
                default:
                    anti_flood = IRC_SERVER_OPTION_INTEGER(server, IRC_SERVER_OPTION_ANTI_FLOOD_PRIO_LOW);
                    break;
            }
            
            add_to_queue = 0;
            if ((queue_msg > 0)
                && (server->outqueue[queue_msg - 1]
                    || ((anti_flood > 0)
                        && (time_now - server->last_user_message < anti_flood))))
            {
                add_to_queue = queue_msg;
            }
            
            tags_to_send = irc_server_get_tags_to_send (tags);
            
            ptr_redirect = irc_redirect_search_available (server);
            
            if (add_to_queue > 0)
            {
                 
                irc_server_outqueue_add (server, add_to_queue - 1, command,
                                         (new_msg && first_message) ? message : NULL,
                                         buffer,
                                         (new_msg) ? 1 : 0,
                                         tags_to_send,
                                         ptr_redirect);
            }
            else
            {
                if (first_message)
                {
                    irc_raw_print (server, IRC_RAW_FLAG_SEND, message);
                }
                if (new_msg)
                {
                    irc_raw_print (server,
                                   IRC_RAW_FLAG_SEND | IRC_RAW_FLAG_MODIFIED,
                                   ptr_msg);
                }
                
                 
                irc_server_send_signal (server, "irc_out",
                                        (command) ? command : "unknown",
                                        ptr_msg,
                                        NULL);
                irc_server_send_signal (server, "irc_outtags",
                                        (command) ? command : "unknown",
                                        ptr_msg,
                                        (tags_to_send) ? tags_to_send : "");
                
                if (irc_server_send (server, buffer, strlen (buffer)) <= 0)
                    rc = 0;
                else
                {
                    if (queue_msg > 0)
                        server->last_user_message = time_now;
                }
                if (ptr_redirect)
                    irc_redirect_init_command (ptr_redirect, buffer);
            }
            
            if (tags_to_send)
                    free (tags_to_send);
            
            if (pos)
            {
                pos[0] = '\n';
                ptr_msg = pos + 1;
            }
            else
                ptr_msg = NULL;
            
            first_message = 0;
        }
        if (msg_encoded)
            free (msg_encoded);
    }
    else
    {
        irc_raw_print (server, IRC_RAW_FLAG_SEND | IRC_RAW_FLAG_MODIFIED,
                       _("(message dropped)"));
    }
    
    if (nick)
        free (nick);
    if (command)
        free (command);
    if (channel)
        free (channel);
    if (new_msg)
        free (new_msg);
    
    return rc;
}