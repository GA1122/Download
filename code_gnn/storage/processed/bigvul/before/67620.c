MODRET set_maxpasswordsize(cmd_rec *cmd) {
  config_rec *c;
  size_t password_len;
  char *len, *ptr = NULL;

  CHECK_ARGS(cmd, 1);
  CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL);

  len = cmd->argv[1];
  if (*len == '-') {
    CONF_ERROR(cmd, "badly formatted parameter");
  }

  password_len = strtoul(len, &ptr, 10);
  if (ptr && *ptr) {
    CONF_ERROR(cmd, "badly formatted parameter");
  }

 

  c = add_config_param(cmd->argv[0], 1, NULL);
  c->argv[0] = palloc(c->pool, sizeof(size_t));
  *((size_t *) c->argv[0]) = password_len;

  return PR_HANDLED(cmd);
}
