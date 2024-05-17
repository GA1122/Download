MODRET set_timeoutsession(cmd_rec *cmd) {
  int timeout = 0, precedence = 0;
  config_rec *c = NULL;

  int ctxt = (cmd->config && cmd->config->config_type != CONF_PARAM ?
     cmd->config->config_type : cmd->server->config_type ?
     cmd->server->config_type : CONF_ROOT);

   
  if (cmd->argc-1 != 1 &&
      cmd->argc-1 != 3) {
    CONF_ERROR(cmd, "missing parameters");
  }

  CHECK_CONF(cmd, CONF_ROOT|CONF_VIRTUAL|CONF_GLOBAL|CONF_ANON);

   
  if (ctxt & CONF_GLOBAL) {
    precedence = 1;

   
  } else if ((ctxt & CONF_ROOT) ||
             (ctxt & CONF_VIRTUAL)) {
    precedence = 2;

  } else if (ctxt & CONF_ANON) {
    precedence = 3;
  }

  if (pr_str_get_duration(cmd->argv[1], &timeout) < 0) {
    CONF_ERROR(cmd, pstrcat(cmd->tmp_pool, "error parsing timeout value '",
      cmd->argv[1], "': ", strerror(errno), NULL));
  }

  if (timeout == 0) {
     
    return PR_HANDLED(cmd);
  }

  if (cmd->argc-1 == 3) {
    if (strncmp(cmd->argv[2], "user", 5) == 0 ||
        strncmp(cmd->argv[2], "group", 6) == 0 ||
        strncmp(cmd->argv[2], "class", 6) == 0) {

        

     } else {
       CONF_ERROR(cmd, pstrcat(cmd->tmp_pool, cmd->argv[0],
         ": unknown classifier used: '", cmd->argv[2], "'", NULL));
    }
  }

  if (cmd->argc-1 == 1) {
    c = add_config_param(cmd->argv[0], 2, NULL);
    c->argv[0] = pcalloc(c->pool, sizeof(int));
    *((int *) c->argv[0]) = timeout;
    c->argv[1] = pcalloc(c->pool, sizeof(unsigned int));
    *((unsigned int *) c->argv[1]) = precedence;

  } else if (cmd->argc-1 == 3) {
    array_header *acl = NULL;
    unsigned int argc;
    void **argv;

    argc = cmd->argc - 3;
    argv = cmd->argv + 2;

    acl = pr_expr_create(cmd->tmp_pool, &argc, (char **) argv);

    c = add_config_param(cmd->argv[0], 0);
    c->argc = argc + 2;

     
    c->argv = pcalloc(c->pool, ((argc + 4) * sizeof(void *)));

     
    argv = c->argv;

     
    *argv = pcalloc(c->pool, sizeof(int));
    *((int *) *argv++) = timeout;

     
    *argv = pcalloc(c->pool, sizeof(unsigned int));
    *((unsigned int *) *argv++) = precedence;

     
    *argv++ = pstrdup(c->pool, cmd->argv[2]);

     
    if (argc && acl) {
      while (argc--) {
        *argv++ = pstrdup(c->pool, *((char **) acl->elts));
        acl->elts = ((char **) acl->elts) + 1;
      }
    }

     
    *argv = NULL;

  } else {
     
    CONF_ERROR(cmd, "wrong number of parameters");
  }

  c->flags |= CF_MERGEDOWN_MULTI;
  return PR_HANDLED(cmd);
}
