MODRET auth_log_pass(cmd_rec *cmd) {

   
  pr_log_auth(PR_LOG_INFO, "%s %s: Login successful.",
    (session.anon_config != NULL) ? "ANON" : C_USER, session.user);

  if (cmd->arg != NULL) {
    size_t passwd_len;

     
    passwd_len = strlen(cmd->arg);
    pr_memscrub(cmd->arg, passwd_len);
  }

  return PR_DECLINED(cmd);
}
