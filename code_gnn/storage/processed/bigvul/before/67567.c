MODRET auth_err_pass(cmd_rec *cmd) {
  const char *user;

  user = pr_table_get(session.notes, "mod_auth.orig-user", NULL);
  if (user != NULL) {
    login_failed(cmd->tmp_pool, user);
  }

   
  pr_table_remove(session.notes, "mod_auth.orig-user", NULL);

  return PR_HANDLED(cmd);
}
