static const char *register_check_user_id_hook(cmd_parms *cmd, void *_cfg,
                                               const char *file,
                                               const char *function,
                                               const char *when)
{
    int apr_hook_when = APR_HOOK_MIDDLE;
 
    return register_named_file_function_hook("check_user_id", cmd, _cfg, file,
                                             function, apr_hook_when);
}
