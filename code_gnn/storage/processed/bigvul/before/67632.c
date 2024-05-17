MODRET set_wtmplog(cmd_rec *cmd) {
  int use_wtmp = -1;
  config_rec *c = NULL;

  CHECK_ARGS(cmd, 1);
  CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_ANON);

  if (strcasecmp(cmd->argv[1], "NONE") == 0) {
    use_wtmp = FALSE;

  } else {
    use_wtmp = get_boolean(cmd, 1);
    if (use_wtmp == -1) {
      CONF_ERROR(cmd, "expected Boolean parameter");
    }
  }

  c = add_config_param(cmd->argv[0], 1, NULL);
  c->argv[0] = pcalloc(c->pool, sizeof(unsigned char));
  *((unsigned char *) c->argv[0]) = use_wtmp;
  c->flags |= CF_MERGEDOWN;

  return PR_HANDLED(cmd);
}
