irc_ctcp_get_reply (struct t_irc_server *server, const char *ctcp)
{
    struct t_config_option *ptr_option;
    char option_name[512];

    snprintf (option_name, sizeof (option_name), "%s.%s", server->name, ctcp);

     
    ptr_option = weechat_config_search_option (irc_config_file,
                                               irc_config_section_ctcp,
                                               option_name);
    if (ptr_option)
        return weechat_config_string (ptr_option);

     
    ptr_option = weechat_config_search_option (irc_config_file,
                                               irc_config_section_ctcp,
                                               ctcp);
    if (ptr_option)
        return weechat_config_string (ptr_option);

     
    return irc_ctcp_get_default_reply (ctcp);
}
