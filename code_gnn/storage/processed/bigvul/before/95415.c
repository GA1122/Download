static int setup_env(pool *p, cmd_rec *cmd, char *user, char *pass) {
  struct passwd *pw;
  config_rec *c, *tmpc;
  char *origuser, *ourname = NULL, *anonname = NULL, *anongroup = NULL, *ugroup = NULL;
  char *defaulttransfermode, *defroot = NULL,*defchdir = NULL,*xferlog = NULL;
  const char *sess_ttyname;
  int aclp, i, res = 0, allow_chroot_symlinks = TRUE, showsymlinks;
  unsigned char *wtmp_log = NULL, *anon_require_passwd = NULL;

   

  session.hide_password = TRUE;

  origuser = user;
  c = pr_auth_get_anon_config(p, &user, &ourname, &anonname);

  if (c)
    session.anon_config = c;

  if (!user) {
    pr_log_auth(PR_LOG_NOTICE, "USER %s: user is not a UserAlias from %s [%s] "
      "to %s:%i", origuser, session.c->remote_name,
      pr_netaddr_get_ipstr(session.c->remote_addr),
      pr_netaddr_get_ipstr(session.c->local_addr), session.c->local_port);
    goto auth_failure;
  }

  pw = pr_auth_getpwnam(p, user);
  if (pw == NULL &&
      c != NULL &&
      ourname != NULL) {
     
    pr_trace_msg("auth", 16,
      "no user entry found for <Anonymous> alias '%s', using '%s'", user,
      ourname);
    pw = pr_auth_getpwnam(p, ourname);
  }

  if (pw == NULL) {
    int auth_code = PR_AUTH_NOPWD;

    pr_log_auth(PR_LOG_NOTICE,
      "USER %s: no such user found from %s [%s] to %s:%i",
      user, session.c->remote_name,
      pr_netaddr_get_ipstr(session.c->remote_addr),
      pr_netaddr_get_ipstr(session.c->local_addr), session.c->local_port);
    pr_event_generate("mod_auth.authentication-code", &auth_code); 

    goto auth_failure;
  }

   
  pw = passwd_dup(p, pw);

  if (pw->pw_uid == PR_ROOT_UID) {
    unsigned char *root_allow = NULL;

    pr_event_generate("mod_auth.root-login", NULL);

     
    if ((root_allow = get_param_ptr(c ? c->subset : main_server->conf,
        "RootLogin", FALSE)) == NULL || *root_allow != TRUE) {
      if (pass) {
        pr_memscrub(pass, strlen(pass));
      }

      pr_log_auth(PR_LOG_NOTICE, "SECURITY VIOLATION: Root login attempted");
      return 0;
    }
  }

  session.user = pstrdup(p, pw->pw_name);
  session.group = pstrdup(p, pr_auth_gid2name(p, pw->pw_gid));

   
  session.login_uid = pw->pw_uid;
  session.login_gid = pw->pw_gid;

   
  pw->pw_dir = path_subst_uservar(p, &pw->pw_dir);

   
  if (strcmp(pw->pw_name, user) != 0) {
    pr_log_debug(DEBUG10, "local user name '%s' differs from client-sent "
      "user name '%s', clearing cached group data", pw->pw_name, user);
    session.gids = NULL;
    session.groups = NULL;
  }

  if (!session.gids &&
      !session.groups) {
     
     res = pr_auth_getgroups(p, pw->pw_name, &session.gids, &session.groups);
     if (res < 1) {
       pr_log_debug(DEBUG2, "no supplemental groups found for user '%s'",
         pw->pw_name);
     }
  }

  tmpc = find_config(main_server->conf, CONF_PARAM, "AllowChrootSymlinks",
    FALSE);
  if (tmpc != NULL) {
    allow_chroot_symlinks = *((int *) tmpc->argv[0]);
  }

   
  aclp = login_check_limits(main_server->conf, FALSE, TRUE, &i);

  if (c) {
    anongroup = get_param_ptr(c->subset, "GroupName", FALSE);
    if (!anongroup)
      anongroup = get_param_ptr(main_server->conf, "GroupName",FALSE);

     
#ifdef PR_USE_REGEX
    if ((tmpc = find_config(c->subset, CONF_PARAM, "AnonRejectPasswords",
        FALSE)) != NULL) {
      int re_res;
      pr_regex_t *pw_regex = (pr_regex_t *) tmpc->argv[0];

      if (pw_regex && pass &&
          ((re_res = pr_regexp_exec(pw_regex, pass, 0, NULL, 0, 0, 0)) == 0)) {
        char errstr[200] = {'\0'};

        pr_regexp_error(re_res, pw_regex, errstr, sizeof(errstr));
        pr_log_auth(PR_LOG_NOTICE, "ANON %s: AnonRejectPasswords denies login",
          origuser);
 
        pr_event_generate("mod_auth.anon-reject-passwords", session.c);
        goto auth_failure;
      }
    }
#endif

    if (!login_check_limits(c->subset, FALSE, TRUE, &i) || (!aclp && !i) ){
      pr_log_auth(PR_LOG_NOTICE, "ANON %s (Login failed): Limit access denies "
        "login", origuser);
      goto auth_failure;
    }
  }

  if (c == NULL &&
      aclp == 0) {
    pr_log_auth(PR_LOG_NOTICE,
      "USER %s (Login failed): Limit access denies login", origuser);
    goto auth_failure;
  }

  if (c) {
    anon_require_passwd = get_param_ptr(c->subset, "AnonRequirePassword",
      FALSE);
  }

  if (!c ||
      (anon_require_passwd && *anon_require_passwd == TRUE)) {
    int auth_code;
    char *user_name = user;

    if (c &&
        origuser &&
        strcasecmp(user, origuser) != 0) {
      unsigned char *auth_using_alias = get_param_ptr(c->subset,
        "AuthUsingAlias", FALSE);

       
      if (auth_using_alias &&
          *auth_using_alias == TRUE) {
        user_name = origuser;
        pr_log_auth(PR_LOG_INFO,
          "ANON AUTH: User %s, authenticating using alias %s", user,
          user_name);
      }
    }

     
    if (!authenticated_without_pass) {
      auth_code = _do_auth(p, c ? c->subset : main_server->conf, user_name,
        pass);

    } else {
      auth_code = PR_AUTH_OK_NO_PASS;
    }

    pr_event_generate("mod_auth.authentication-code", &auth_code);

    if (auth_code < 0) {
       

      c = _auth_group(p, user, &anongroup, &ourname, &anonname, pass);
      if (c != NULL) {
        if (c->config_type != CONF_ANON) {
          c = NULL;
          ugroup = anongroup;
          anongroup = NULL;
        }

        auth_code = PR_AUTH_OK;
      }
    }

    if (pass)
      pr_memscrub(pass, strlen(pass));

    if (session.auth_mech)
      pr_log_debug(DEBUG2, "user '%s' authenticated by %s", user,
        session.auth_mech);

    switch (auth_code) {
      case PR_AUTH_OK_NO_PASS:
        auth_pass_resp_code = R_232;
        break;

      case PR_AUTH_OK:
        auth_pass_resp_code = R_230;
        break;

      case PR_AUTH_NOPWD:
        pr_log_auth(PR_LOG_NOTICE,
          "USER %s (Login failed): No such user found", user);
        goto auth_failure;

      case PR_AUTH_BADPWD:
        pr_log_auth(PR_LOG_NOTICE,
          "USER %s (Login failed): Incorrect password", origuser);
        goto auth_failure;

      case PR_AUTH_AGEPWD:
        pr_log_auth(PR_LOG_NOTICE, "USER %s (Login failed): Password expired",
          user);
        goto auth_failure;

      case PR_AUTH_DISABLEDPWD:
        pr_log_auth(PR_LOG_NOTICE, "USER %s (Login failed): Account disabled",
          user);
        goto auth_failure;

      default:
        break;
    };

     
    if (auth_code < 0)
      goto auth_failure;

    if (pw->pw_uid == PR_ROOT_UID) {
      pr_log_auth(PR_LOG_WARNING, "ROOT FTP login successful");
    }

  } else if (c && (!anon_require_passwd || *anon_require_passwd == FALSE)) {
    session.hide_password = FALSE;
  }

  pr_auth_setgrent(p);

  res = pr_auth_is_valid_shell(c ? c->subset : main_server->conf,
    pw->pw_shell);
  if (res == FALSE) {
    pr_log_auth(PR_LOG_NOTICE, "USER %s (Login failed): Invalid shell: '%s'",
      user, pw->pw_shell);
    goto auth_failure;
  }

  res = pr_auth_banned_by_ftpusers(c ? c->subset : main_server->conf,
    pw->pw_name);
  if (res == TRUE) {
    pr_log_auth(PR_LOG_NOTICE, "USER %s (Login failed): User in "
      PR_FTPUSERS_PATH, user);
    goto auth_failure;
  }

  if (c) {
    struct group *grp = NULL;
    unsigned char *add_userdir = NULL;
    char *u, *chroot_dir;

    u = pr_table_get(session.notes, "mod_auth.orig-user", NULL);
    add_userdir = get_param_ptr(c->subset, "UserDirRoot", FALSE);

     

    pr_signals_block();

    PRIVS_ROOT
    res = set_groups(p, pw->pw_gid, session.gids);
    if (res < 0) {
      pr_log_pri(PR_LOG_WARNING, "error: unable to set groups: %s",
        strerror(errno));
    }

#ifndef PR_DEVEL_COREDUMP
# ifdef __hpux
    if (setresuid(0, 0, 0) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setresuid(): %s", strerror(errno));
    }

    if (setresgid(0, 0, 0) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setresgid(): %s", strerror(errno));
    }
# else
    if (setuid(PR_ROOT_UID) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setuid(): %s", strerror(errno));
    }

    if (setgid(PR_ROOT_GID) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setgid(): %s", strerror(errno));
    }
# endif  
#endif  

    PRIVS_SETUP(pw->pw_uid, pw->pw_gid)

    if ((add_userdir && *add_userdir == TRUE) &&
        strcmp(u, user) != 0) {
      chroot_dir = pdircat(p, c->name, u, NULL);

    } else {
      chroot_dir = c->name;
    }

    if (allow_chroot_symlinks == FALSE) {
      char *chroot_path, target_path[PR_TUNABLE_PATH_MAX+1];
      struct stat st;

      chroot_path = chroot_dir;
      if (chroot_path[0] != '/') {
        if (chroot_path[0] == '~') {
          if (pr_fs_interpolate(chroot_path, target_path,
              sizeof(target_path)-1) == 0) {
            chroot_path = target_path;

          } else {
            chroot_path = NULL;
          }
        }
      }

      if (chroot_path != NULL) {
        size_t chroot_pathlen;

         
        chroot_pathlen = strlen(chroot_path);
        if (chroot_pathlen > 1 &&
            chroot_path[chroot_pathlen-1] == '/') {
          chroot_path[chroot_pathlen-1] = '\0';
        }

        pr_fs_clear_cache();
        res = pr_fsio_lstat(chroot_path, &st);
        if (res < 0) {
          int xerrno = errno;

          pr_log_pri(PR_LOG_WARNING, "error: unable to check %s: %s",
            chroot_path, strerror(xerrno));

          errno = xerrno;
          chroot_path = NULL;

        } else {
          if (S_ISLNK(st.st_mode)) {
            pr_log_pri(PR_LOG_WARNING,
              "error: <Anonymous %s> is a symlink (denied by "
              "AllowChrootSymlinks config)", chroot_path);
            errno = EPERM;
            chroot_path = NULL;
          }
        }
      }

      if (chroot_path != NULL) {
        session.chroot_path = dir_realpath(p, chroot_dir);

      } else {
        session.chroot_path = NULL;
      }

      if (session.chroot_path == NULL) {
        pr_log_debug(DEBUG8, "error resolving '%s': %s", chroot_dir,
          strerror(errno));
      }

    } else {
      session.chroot_path = dir_realpath(p, chroot_dir);
      if (session.chroot_path == NULL) {
        pr_log_debug(DEBUG8, "error resolving '%s': %s", chroot_dir,
          strerror(errno));
      }
    }

    if (session.chroot_path &&
        pr_fsio_access(session.chroot_path, X_OK, session.uid,
          session.gid, session.gids) != 0) {
      session.chroot_path = NULL;

    } else {
      session.chroot_path = pstrdup(session.pool, session.chroot_path);
    }

     
    PRIVS_ROOT
    res = set_groups(p, daemon_gid, daemon_gids);
    if (res < 0) {
      pr_log_pri(PR_LOG_ERR, "error: unable to set groups: %s",
        strerror(errno));
    }

#ifndef PR_DEVEL_COREDUMP
# ifdef __hpux
    if (setresuid(0, 0, 0) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setresuid(): %s", strerror(errno));
    }

    if (setresgid(0, 0, 0) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setresgid(): %s", strerror(errno));
    }
# else
    if (setuid(PR_ROOT_UID) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setuid(): %s", strerror(errno));
    }

    if (setgid(PR_ROOT_GID) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setgid(): %s", strerror(errno));
    }
# endif  
#endif  

    PRIVS_SETUP(daemon_uid, daemon_gid)

    pr_signals_unblock();

     
#ifdef HAVE_GETEUID
    if (getegid() != daemon_gid ||
        geteuid() != daemon_uid) {

      PRIVS_RELINQUISH

      pr_log_pri(PR_LOG_WARNING,
        "switching IDs from user %s back to daemon uid/gid failed: %s",
        session.user, strerror(errno));
      pr_session_disconnect(&auth_module, PR_SESS_DISCONNECT_BY_APPLICATION,
        NULL);
    }
#endif  

    if (anon_require_passwd &&
        *anon_require_passwd == TRUE) {
      session.anon_user = pstrdup(session.pool, origuser);

    } else {
      session.anon_user = pstrdup(session.pool, pass);
    }

    if (!session.chroot_path) {
      pr_log_pri(PR_LOG_NOTICE, "%s: Directory %s is not accessible",
        session.user, c->name);
      pr_response_add_err(R_530, _("Unable to set anonymous privileges."));
      goto auth_failure;
    }

    sstrncpy(session.cwd, "/", sizeof(session.cwd));
    xferlog = get_param_ptr(c->subset, "TransferLog", FALSE);

    if (anongroup) {
      grp = pr_auth_getgrnam(p, anongroup);
      if (grp) {
        pw->pw_gid = grp->gr_gid;
        session.group = pstrdup(p, grp->gr_name);
      }
    }

  } else {
    struct group *grp;
    char *homedir;

    if (ugroup) {
      grp = pr_auth_getgrnam(p, ugroup);
      if (grp) {
        pw->pw_gid = grp->gr_gid;
        session.group = pstrdup(p, grp->gr_name);
      }
    }

     
    PRIVS_USER
    homedir = dir_realpath(p, pw->pw_dir);
    PRIVS_RELINQUISH

    if (homedir)
      sstrncpy(session.cwd, homedir, sizeof(session.cwd));
    else
      sstrncpy(session.cwd, pw->pw_dir, sizeof(session.cwd));
  }

   

  if (!c && mkhome) {
    if (create_home(p, session.cwd, origuser, pw->pw_uid, pw->pw_gid) < 0) {

       
      goto auth_failure;
    }
  }

   
  defchdir = get_default_chdir(p, (c ? c->subset : main_server->conf));

  if (defchdir)
    sstrncpy(session.cwd, defchdir, sizeof(session.cwd));

   

  if (!login_check_limits((c ? c->subset : main_server->conf), FALSE, TRUE,
      &i)) {
    pr_log_auth(PR_LOG_NOTICE, "%s %s: Limit access denies login",
      (c != NULL) ? "ANON" : C_USER, origuser);
    goto auth_failure;
  }

   
  resolve_deferred_dirs(main_server);
  fixup_dirs(main_server, CF_DEFER);

   
  if (c && c->subset)
    resolve_anonymous_dirs(c->subset);

   

  sess_ttyname = pr_session_get_ttyname(p);

   
  if (c)
    wtmp_log = get_param_ptr(c->subset, "WtmpLog", FALSE);

  if (wtmp_log == NULL)
    wtmp_log = get_param_ptr(main_server->conf, "WtmpLog", FALSE);

   
#if defined(HAVE_UTMPX_H) && \
    defined(__FreeBSD_version) && __FreeBSD_version >= 900007
  if (wtmp_log == NULL ||
      *wtmp_log == TRUE) {
    wtmp_log = pcalloc(p, sizeof(unsigned char));
    *wtmp_log = FALSE;

    pr_log_debug(DEBUG5,
      "WtpmLog automatically disabled; see Bug#3482 for details");
  }
#endif

  PRIVS_ROOT

  if (wtmp_log == NULL ||
      *wtmp_log == TRUE) {
    log_wtmp(sess_ttyname, session.user, session.c->remote_name,
      session.c->remote_addr);
    session.wtmp_log = TRUE;
  }

#ifdef PR_USE_LASTLOG
  if (lastlog) {
    log_lastlog(pw->pw_uid, session.user, sess_ttyname, session.c->remote_addr);
  }
#endif  

   
  if (!xferlog) {
    if (c)
      xferlog = get_param_ptr(c->subset, "TransferLog", FALSE);

    if (!xferlog)
      xferlog = get_param_ptr(main_server->conf, "TransferLog", FALSE);

    if (!xferlog)
      xferlog = PR_XFERLOG_PATH;
  }

  if (strcasecmp(xferlog, "NONE") == 0) {
    xferlog_open(NULL);

  } else {
    xferlog_open(xferlog);
  }

  res = set_groups(p, pw->pw_gid, session.gids);
  if (res < 0) {
    pr_log_pri(PR_LOG_ERR, "error: unable to set groups: %s",
      strerror(errno));
  }

  PRIVS_RELINQUISH

   
  if (c == NULL) {
    if (get_default_root(session.pool, allow_chroot_symlinks, &defroot) < 0) {
      pr_log_pri(PR_LOG_NOTICE,
        "error: unable to determine DefaultRoot directory");
      pr_response_send(R_530, _("Login incorrect."));
      pr_session_end(0);
    }

    ensure_open_passwd(p);

    if (defroot != NULL) {
      if (pr_auth_chroot(defroot) == -1) {
        pr_log_pri(PR_LOG_NOTICE, "error: unable to set DefaultRoot directory");
        pr_response_send(R_530, _("Login incorrect."));
        pr_session_end(0);
      }

       

      if (strncmp(session.cwd, defroot, strlen(defroot)) == 0) {
        char *newcwd = &session.cwd[strlen(defroot)];

        if (*newcwd == '/')
          newcwd++;
        session.cwd[0] = '/';
        sstrncpy(&session.cwd[1], newcwd, sizeof(session.cwd));
      }
    }
  }

  if (c)
    ensure_open_passwd(p);

  if (c &&
      pr_auth_chroot(session.chroot_path) == -1) {
    pr_log_pri(PR_LOG_NOTICE, "error: unable to set anonymous privileges");
    pr_response_send(R_530, _("Login incorrect."));
    pr_session_end(0);
  }

   

  PRIVS_ROOT

#ifndef PR_DEVEL_COREDUMP
# ifdef __hpux
    if (setresuid(0, 0, 0) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setresuid(): %s", strerror(errno));
    }

    if (setresgid(0, 0, 0) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setresgid(): %s", strerror(errno));
    }
# else
    if (setuid(PR_ROOT_UID) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setuid(): %s", strerror(errno));
    }

    if (setgid(PR_ROOT_GID) < 0) {
      pr_log_pri(PR_LOG_ERR, "unable to setgid(): %s", strerror(errno));
    }
# endif  
#endif  

  PRIVS_SETUP(pw->pw_uid, pw->pw_gid)

#ifdef HAVE_GETEUID
  if (getegid() != pw->pw_gid ||
     geteuid() != pw->pw_uid) {

    PRIVS_RELINQUISH
    pr_log_pri(PR_LOG_ERR, "error: %s setregid() or setreuid(): %s",
      session.user, strerror(errno));
    pr_response_send(R_530, _("Login incorrect."));
    pr_session_end(0);
  }
#endif

   
  if (pw->pw_dir == NULL ||
      strncmp(pw->pw_dir, "", 1) == 0) {
    pr_log_pri(PR_LOG_WARNING, "error: user %s home directory is NULL or \"\"",
      session.user);
    pr_response_send(R_530, _("Login incorrect."));
    pr_session_end(0);
  }

  {
    unsigned char *show_symlinks = get_param_ptr(
      c ? c->subset : main_server->conf, "ShowSymlinks", FALSE);

    if (!show_symlinks || *show_symlinks == TRUE)
      showsymlinks = TRUE;
    else
      showsymlinks = FALSE;
  }

   

   
  if (pr_fsio_chdir_canon(session.cwd, !showsymlinks) == -1) {

     

    if (session.chroot_path != NULL || defroot) {

      pr_log_debug(DEBUG2, "unable to chdir to %s (%s), defaulting to chroot "
        "directory %s", session.cwd, strerror(errno),
        (session.chroot_path ? session.chroot_path : defroot));

      if (pr_fsio_chdir_canon("/", !showsymlinks) == -1) {
        pr_log_pri(PR_LOG_NOTICE, "%s chdir(\"/\") failed: %s", session.user,
          strerror(errno));
        pr_response_send(R_530, _("Login incorrect."));
        pr_session_end(0);
      }

    } else if (defchdir) {

       
      pr_log_debug(DEBUG2, "unable to chdir to %s (%s), defaulting to home "
        "directory %s", session.cwd, strerror(errno), pw->pw_dir);

      if (pr_fsio_chdir_canon(pw->pw_dir, !showsymlinks) == -1) {
        pr_log_pri(PR_LOG_NOTICE, "%s chdir(\"%s\") failed: %s", session.user,
          session.cwd, strerror(errno));
        pr_response_send(R_530, _("Login incorrect."));
        pr_session_end(0);
      }

    } else {

       
      pr_log_pri(PR_LOG_NOTICE, "%s chdir(\"%s\") failed: %s", session.user,
        session.cwd, strerror(errno));
      pr_response_send(R_530, _("Login incorrect."));
      pr_session_end(0);
    }
  }

  sstrncpy(session.cwd, pr_fs_getcwd(), sizeof(session.cwd));
  sstrncpy(session.vwd, pr_fs_getvwd(), sizeof(session.vwd));

   
  dir_check_full(p, cmd, G_NONE, session.cwd, NULL);

  if (c) {
    if (!session.hide_password) {
      session.proc_prefix = pstrcat(session.pool, session.c->remote_name,
        ": anonymous/", pass, NULL);

    } else {
      session.proc_prefix = pstrcat(session.pool, session.c->remote_name,
        ": anonymous", NULL);
    }

    session.sf_flags = SF_ANON;

  } else {
    session.proc_prefix = pstrdup(session.pool, session.c->remote_name);
    session.sf_flags = 0;
  }

   
   

   
  defaulttransfermode = (char *) get_param_ptr(main_server->conf,
    "DefaultTransferMode", FALSE);

  if (defaulttransfermode &&
      strcasecmp(defaulttransfermode, "binary") == 0) {
    session.sf_flags &= (SF_ALL^SF_ASCII);

  } else {
    session.sf_flags |= SF_ASCII;
  }

   

   
  pr_scoreboard_entry_update(session.pid,
    PR_SCORE_USER, session.user,
    PR_SCORE_CWD, session.cwd,
    NULL);

  pr_session_set_idle();

  pr_timer_remove(PR_TIMER_LOGIN, &auth_module);

   
  session.user = pstrdup(session.pool, session.user);

  if (session.group)
    session.group = pstrdup(session.pool, session.group);

  if (session.gids)
    session.gids = copy_array(session.pool, session.gids);

   
  session.groups = copy_array_str(session.pool, session.groups);

   
  pr_resolve_fs_map();

  return 1;

auth_failure:
  if (pass)
    pr_memscrub(pass, strlen(pass));
  session.user = session.group = NULL;
  session.gids = session.groups = NULL;
  session.wtmp_log = FALSE;
  return 0;
}