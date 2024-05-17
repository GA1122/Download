static const char *set_errorlog(cmd_parms *cmd, void *dummy, const char *arg1,
                                const char *arg2)
{
    ap_errorlog_provider *provider;
    const char *err;
    cmd->server->errorlog_provider = NULL;

    if (!arg2) {
         
        if (strncmp("syslog", arg1, 6) == 0) {
            arg2 = arg1 + 7;  
            arg1 = "syslog";
        }
        else {
             
            provider = ap_lookup_provider(AP_ERRORLOG_PROVIDER_GROUP, arg1,
                                          AP_ERRORLOG_PROVIDER_VERSION);
            if (provider) {
                arg2 = "";
            }
            else {
                return set_server_string_slot(cmd, dummy, arg1);
            }
        }
    }

    if (strcmp("file", arg1) == 0) {
        return set_server_string_slot(cmd, dummy, arg2);
    }

    provider = ap_lookup_provider(AP_ERRORLOG_PROVIDER_GROUP, arg1,
                                    AP_ERRORLOG_PROVIDER_VERSION);
    if (!provider) {
        return apr_psprintf(cmd->pool,
                            "Unknown ErrorLog provider: %s",
                            arg1);
    }

    err = provider->parse_errorlog_arg(cmd, arg2);
    if (err) {
        return err;
    }

    cmd->server->errorlog_provider = provider;
    return set_server_string_slot(cmd, dummy, arg2);
}
