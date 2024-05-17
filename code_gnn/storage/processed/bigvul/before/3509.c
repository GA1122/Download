irc_server_sasl_enabled (struct t_irc_server *server)
{
    const char *sasl_username, *sasl_password;
    
    sasl_username = IRC_SERVER_OPTION_STRING(server,
                                             IRC_SERVER_OPTION_SASL_USERNAME);
    sasl_password = IRC_SERVER_OPTION_STRING(server,
                                             IRC_SERVER_OPTION_SASL_PASSWORD);
    
     
    return (sasl_username && sasl_username[0]
            && sasl_password && sasl_password[0]) ? 1 : 0;
}
