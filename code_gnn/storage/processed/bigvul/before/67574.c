MODRET auth_post_pass(cmd_rec *cmd) {
  config_rec *c = NULL;
  const char *grantmsg = NULL, *user;
  unsigned int ctxt_precedence = 0;
  unsigned char have_user_timeout, have_group_timeout, have_class_timeout,
    have_all_timeout, *root_revoke = NULL, *authenticated;
  struct stat st;

   
  authenticated = get_param_ptr(cmd->server->conf, "authenticated", FALSE);

   
  pr_auth_clear_auth_only_modules();

  if (authenticated != NULL &&
      *authenticated == TRUE) {

     
    c = find_config(main_server->conf, CONF_PARAM, "Protocols", FALSE);
    if (c != NULL) {
      register unsigned int i;
      array_header *protocols;
      char **elts;
      const char *protocol;

      protocols = c->argv[0];
      elts = protocols->elts;

      protocol = pr_session_get_protocol(PR_SESS_PROTO_FL_LOGOUT);

       
      if (session.rfc2228_mech == NULL &&
          strncmp(protocol, "SSH2", 5) != 0) {
        int allow_ftp = FALSE;

        for (i = 0; i < protocols->nelts; i++) {
          char *proto;

          proto = elts[i];
          if (proto != NULL) {
            if (strncasecmp(proto, "ftp", 4) == 0) {
              allow_ftp = TRUE;
              break;
            }
          }
        }

        if (!allow_ftp) {
          pr_log_debug(DEBUG0, "%s", "ftp protocol denied by Protocols config");
          pr_response_send(R_530, "%s", _("Login incorrect."));
          pr_session_disconnect(&auth_module, PR_SESS_DISCONNECT_CONFIG_ACL,
            "Denied by Protocols setting");
        }
      }
    }
  }

  user = pr_table_get(session.notes, "mod_auth.orig-user", NULL);

   
  auth_count_scoreboard(cmd, session.user);

   
  if (pr_fsio_stat(session.cwd, &st) != -1)
    build_dyn_config(cmd->tmp_pool, session.cwd, &st, TRUE);

  have_user_timeout = have_group_timeout = have_class_timeout =
    have_all_timeout = FALSE;

  c = find_config(TOPLEVEL_CONF, CONF_PARAM, "TimeoutSession", FALSE);
  while (c != NULL) {
    pr_signals_handle();

    if (c->argc == 3) {
      if (strncmp(c->argv[1], "user", 5) == 0) {
        if (pr_expr_eval_user_or((char **) &c->argv[2]) == TRUE) {

          if (*((unsigned int *) c->argv[1]) > ctxt_precedence) {

             
            ctxt_precedence = *((unsigned int *) c->argv[1]);

            TimeoutSession = *((int *) c->argv[0]);

            have_group_timeout = have_class_timeout = have_all_timeout = FALSE;
            have_user_timeout = TRUE;
          }
        }

      } else if (strncmp(c->argv[1], "group", 6) == 0) {
        if (pr_expr_eval_group_and((char **) &c->argv[2]) == TRUE) {

          if (*((unsigned int *) c->argv[1]) > ctxt_precedence) {

             
            ctxt_precedence = *((unsigned int *) c->argv[1]);

            TimeoutSession = *((int *) c->argv[0]);

            have_user_timeout = have_class_timeout = have_all_timeout = FALSE;
            have_group_timeout = TRUE;
          }
        }

      } else if (strncmp(c->argv[1], "class", 6) == 0) {
        if (session.conn_class != NULL &&
            strcmp(session.conn_class->cls_name, c->argv[2]) == 0) {

          if (*((unsigned int *) c->argv[1]) > ctxt_precedence) {

             
            ctxt_precedence = *((unsigned int *) c->argv[1]);

            TimeoutSession = *((int *) c->argv[0]);

            have_user_timeout = have_group_timeout = have_all_timeout = FALSE;
            have_class_timeout = TRUE;
          }
        }
      }

    } else {

      if (*((unsigned int *) c->argv[1]) > ctxt_precedence) {

         
        ctxt_precedence = *((unsigned int *) c->argv[1]);

        TimeoutSession = *((int *) c->argv[0]);

        have_user_timeout = have_group_timeout = have_class_timeout = FALSE;
        have_all_timeout = TRUE;
      }
    }

    c = find_config_next(c, c->next, CONF_PARAM, "TimeoutSession", FALSE);
  }

   

  if (have_user_timeout || have_group_timeout ||
      have_class_timeout || have_all_timeout) {
    pr_log_debug(DEBUG4, "setting TimeoutSession of %d seconds for current %s",
      TimeoutSession,
      have_user_timeout ? "user" : have_group_timeout ? "group" :
      have_class_timeout ? "class" : "all");
    pr_timer_add(TimeoutSession, PR_TIMER_SESSION, &auth_module,
      auth_session_timeout_cb, "TimeoutSession");
  }

   
  if (displaylogin_fh) {
    if (!(session.sf_flags & SF_ANON)) {
      if (pr_display_fh(displaylogin_fh, NULL, auth_pass_resp_code, 0) < 0) {
        pr_log_debug(DEBUG6, "unable to display DisplayLogin file '%s': %s",
          displaylogin_fh->fh_path, strerror(errno));
      }

      pr_fsio_close(displaylogin_fh);
      displaylogin_fh = NULL;

    } else {
       
      char *displaylogin;

      pr_fsio_close(displaylogin_fh);
      displaylogin_fh = NULL;

      displaylogin = get_param_ptr(TOPLEVEL_CONF, "DisplayLogin", FALSE);
      if (displaylogin) {
        if (pr_display_file(displaylogin, NULL, auth_pass_resp_code, 0) < 0) {
          pr_log_debug(DEBUG6, "unable to display DisplayLogin file '%s': %s",
            displaylogin, strerror(errno));
        }
      }
    }

  } else {
    char *displaylogin = get_param_ptr(TOPLEVEL_CONF, "DisplayLogin", FALSE);
    if (displaylogin) {
      if (pr_display_file(displaylogin, NULL, auth_pass_resp_code, 0) < 0) {
        pr_log_debug(DEBUG6, "unable to display DisplayLogin file '%s': %s",
          displaylogin, strerror(errno));
      }
    }
  }

  grantmsg = get_param_ptr(TOPLEVEL_CONF, "AccessGrantMsg", FALSE);
  if (grantmsg == NULL) {
     
    if (session.sf_flags & SF_ANON) {
      pr_response_add(auth_pass_resp_code, "%s",
        _("Anonymous access granted, restrictions apply"));

    } else {
      pr_response_add(auth_pass_resp_code, _("User %s logged in"), user);
    }

  } else {
      
     grantmsg = sreplace(cmd->tmp_pool, grantmsg, "%u", user, NULL);
     pr_response_add(auth_pass_resp_code, "%s", grantmsg);
  }

  login_succeeded(cmd->tmp_pool, user);

   
  root_revoke = get_param_ptr(TOPLEVEL_CONF, "RootRevoke", FALSE);
  if (root_revoke != NULL &&
      *root_revoke > 0) {
    pr_signals_block();
    PRIVS_ROOT
    PRIVS_REVOKE
    pr_signals_unblock();

     
    session.disable_id_switching = TRUE;

    if (*root_revoke == 1) {
       
      if (session.c->local_port < 1024) {
        pr_log_debug(DEBUG0,
          "RootRevoke in effect, active data transfers may not succeed");
      }
    }

    pr_log_debug(DEBUG0, "RootRevoke in effect, dropped root privs");
  }

  c = find_config(TOPLEVEL_CONF, CONF_PARAM, "AnonAllowRobots", FALSE);
  if (c != NULL) {
    auth_anon_allow_robots = *((int *) c->argv[0]);
  }

  return PR_DECLINED(cmd);
}
