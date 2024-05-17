irc_server_timer_sasl_cb (void *data, int remaining_calls)
{
    struct t_irc_server *server;
    
     
    (void) remaining_calls;
    
    server = (struct t_irc_server *)data;
    
    if (!server)
        return WEECHAT_RC_ERROR;
    
    server->hook_timer_sasl = NULL;
    
    if (!server->is_connected)
    {
        weechat_printf (server->buffer,
                        _("%s%s: sasl authentication timeout"),
                        weechat_prefix ("error"), IRC_PLUGIN_NAME);
        irc_server_sendf (server, 0, NULL, "CAP END");
    }
    
    return WEECHAT_RC_OK;
}
