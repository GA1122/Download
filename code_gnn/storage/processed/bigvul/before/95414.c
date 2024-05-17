MODRET set_useralias(cmd_rec *cmd) {
  config_rec *c = NULL;

  CHECK_ARGS(cmd, 2);
  CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_ANON);

   
  if (strcmp(cmd->argv[1], cmd->argv[2]) == 0)
    CONF_ERROR(cmd, "alias and real user names must differ");

  c = add_config_param_str(cmd->argv[0], 2, cmd->argv[1], cmd->argv[2]);

   
  if (!check_context(cmd, CONF_ANON)) {
    c->flags |= CF_MERGEDOWN_MULTI;
  }

  return PR_HANDLED(cmd);
}