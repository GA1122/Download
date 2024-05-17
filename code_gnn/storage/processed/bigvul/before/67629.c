MODRET set_useralias(cmd_rec *cmd) {
  config_rec *c = NULL;
  char *alias, *real_user;

  CHECK_ARGS(cmd, 2);
  CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_ANON);

   
  alias = cmd->argv[1];
  real_user = cmd->argv[2];

  if (strcmp(alias, real_user) == 0) {
    CONF_ERROR(cmd, "alias and real user names must differ");
  }

  c = add_config_param_str(cmd->argv[0], 2, alias, real_user);

   
  if (!check_context(cmd, CONF_ANON)) {
    c->flags |= CF_MERGEDOWN_MULTI;
  }

  return PR_HANDLED(cmd);
}
