irc_server_msgq_flush ()
{
    struct t_irc_message *next;
    char *ptr_data, *new_msg, *ptr_msg, *pos;
    char *nick, *host, *command, *channel, *arguments;
    char *msg_decoded, *msg_decoded_without_color;
    char str_modifier[64], modifier_data[256];
    
    while (irc_recv_msgq)
    {
        if (irc_recv_msgq->data)
        {
            ptr_data = irc_recv_msgq->data;
            while (ptr_data[0] == ' ')
            {
                ptr_data++;
            }
            
            if (ptr_data[0])
            {
                irc_raw_print (irc_recv_msgq->server, IRC_RAW_FLAG_RECV,
                               ptr_data);
                
                irc_message_parse (ptr_data, NULL, NULL, &command, NULL, NULL);
                snprintf (str_modifier, sizeof (str_modifier),
                          "irc_in_%s",
                          (command) ? command : "unknown");
                new_msg = weechat_hook_modifier_exec (str_modifier,
                                                      irc_recv_msgq->server->name,
                                                      ptr_data);
                if (command)
                    free (command);
                
                 
                if (new_msg && (strcmp (ptr_data, new_msg) == 0))
                {
                    free (new_msg);
                    new_msg = NULL;
                }
                
                 
                if (!new_msg || new_msg[0])
                {
                     
                    ptr_msg = (new_msg) ? new_msg : ptr_data;
                    
                    while (ptr_msg && ptr_msg[0])
                    {
                        pos = strchr (ptr_msg, '\n');
                        if (pos)
                            pos[0] = '\0';
                        
                        if (new_msg)
                        {
                            irc_raw_print (irc_recv_msgq->server,
                                           IRC_RAW_FLAG_RECV | IRC_RAW_FLAG_MODIFIED,
                                           ptr_msg);
                        }
                        
                        irc_message_parse (ptr_msg, &nick, &host, &command,
                                           &channel, &arguments);
                        
                         
                        if (channel && irc_channel_is_channel (channel))
                        {
                            snprintf (modifier_data, sizeof (modifier_data),
                                      "%s.%s.%s",
                                      weechat_plugin->name,
                                      irc_recv_msgq->server->name,
                                      channel);
                        }
                        else
                        {
                            if (nick && (!host || (strcmp (nick, host) != 0)))
                            {
                                snprintf (modifier_data, sizeof (modifier_data),
                                          "%s.%s.%s",
                                          weechat_plugin->name,
                                          irc_recv_msgq->server->name,
                                          nick);
                            }
                            else
                            {
                                snprintf (modifier_data, sizeof (modifier_data),
                                          "%s.%s",
                                          weechat_plugin->name,
                                          irc_recv_msgq->server->name);
                            }
                        }
                        msg_decoded = weechat_hook_modifier_exec ("charset_decode",
                                                                  modifier_data,
                                                                  ptr_msg);
                        
                         
                        msg_decoded_without_color =
                            weechat_string_remove_color ((msg_decoded) ? msg_decoded : ptr_msg,
                                                         "?");
                        
                         
                        if (irc_redirect_message (irc_recv_msgq->server,
                                                  (msg_decoded_without_color) ?
                                                  msg_decoded_without_color : ((msg_decoded) ? msg_decoded : ptr_msg),
                                                  command, arguments))
                        {
                             
                        }
                        else
                        {
                             
                            irc_protocol_recv_command (irc_recv_msgq->server,
                                                       (msg_decoded_without_color) ?
                                                       msg_decoded_without_color : ((msg_decoded) ? msg_decoded : ptr_msg),
                                                       command,
                                                       channel);
                        }
                        
                        if (nick)
                            free (nick);
                        if (host)
                            free (host);
                        if (command)
                            free (command);
                        if (channel)
                            free (channel);
                        if (arguments)
                            free (arguments);
                        if (msg_decoded)
                            free (msg_decoded);
                        if (msg_decoded_without_color)
                            free (msg_decoded_without_color);
                        
                        if (pos)
                        {
                            pos[0] = '\n';
                            ptr_msg = pos + 1;
                        }
                        else
                            ptr_msg = NULL;
                    }
                }
                else
                {
                    irc_raw_print (irc_recv_msgq->server,
                                   IRC_RAW_FLAG_RECV | IRC_RAW_FLAG_MODIFIED,
                                   _("(message dropped)"));
                }
                if (new_msg)
                    free (new_msg);
            }
            free (irc_recv_msgq->data);
        }
        
        next = irc_recv_msgq->next_message;
        free (irc_recv_msgq);
        irc_recv_msgq = next;
        if (!irc_recv_msgq)
            irc_msgq_last_msg = NULL;
    }
}
