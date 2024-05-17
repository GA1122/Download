static const char *generate_message(cmd_parms *cmd, void *dummy,
                                    const char *arg)
{
     
    int level = (cmd->info==(void*)APLOG_ERR)? APLOG_ERR: APLOG_WARNING;
    char * msg;

     
    ap_configfile_t * cf = cmd->config_file;
    ap_directive_t const * ed1 = cmd->directive;
    ap_directive_t const * ed2 = cmd->err_directive;

     
    if (!arg || !*arg) {
        return "The Error or Warning directive was used with no message.";
    }

     
    msg = (char *)arg;
    if (*arg == '"' || *arg == '\'') {
        apr_size_t len = strlen(arg);
        char last = *(arg + len - 1);

        if (*arg == last) {
            msg = apr_pstrndup(cmd->pool, arg + 1, len - 2);
        }
    }

     
    ap_log_error(APLOG_MARK, level, 0, NULL,
                 "%s on line %d of %s", msg,
                 cf? cf->line_number:
                   ed1? ed1->line_num:
                     ed2? ed2->line_num: -1,
                 cf? cf->name:
                   ed1? ed1->filename:
                     ed2? ed2->filename: "<UNKNOWN>");

     
    return level==APLOG_ERR?
        "Configuration processing stopped by Error directive": NULL;
}
