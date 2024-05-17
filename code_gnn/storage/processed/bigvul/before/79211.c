static const char *StreamTcpParseOSPolicy (char *conf_var_name)
{
    SCEnter();
    char conf_var_type_name[15] = "host-os-policy";
    char *conf_var_full_name = NULL;
    const char *conf_var_value = NULL;

    if (conf_var_name == NULL)
        goto end;

     
    conf_var_full_name = (char *)SCMalloc(strlen(conf_var_type_name) +
                                        strlen(conf_var_name) + 2);
    if (conf_var_full_name == NULL)
        goto end;

    if (snprintf(conf_var_full_name,
                 strlen(conf_var_type_name) + strlen(conf_var_name) + 2, "%s.%s",
                 conf_var_type_name, conf_var_name) < 0) {
        SCLogError(SC_ERR_INVALID_VALUE, "Error in making the conf full name");
        goto end;
    }

    if (ConfGet(conf_var_full_name, &conf_var_value) != 1) {
        SCLogError(SC_ERR_UNKNOWN_VALUE, "Error in getting conf value for conf name %s",
                    conf_var_full_name);
        goto end;
    }

    SCLogDebug("Value obtained from the yaml conf file, for the var "
               "\"%s\" is \"%s\"", conf_var_name, conf_var_value);

 end:
    if (conf_var_full_name != NULL)
        SCFree(conf_var_full_name);
    SCReturnCharPtr(conf_var_value);


}