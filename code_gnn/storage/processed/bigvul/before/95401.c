MODRET auth_post_host(cmd_rec *cmd) {

   
  if (session.prev_server != NULL) {
    int res;

     
    pr_timer_remove(PR_TIMER_LOGIN, &auth_module);

    pr_event_unregister(&auth_module, "core.exit", auth_exit_ev);

     
    mkhome = FALSE;

#ifdef PR_USE_LASTLOG
     
    lastlog = FALSE;
#endif  

    res = auth_sess_init();
    if (res < 0) {
      pr_session_disconnect(&auth_module,
        PR_SESS_DISCONNECT_SESSION_INIT_FAILED, NULL);
    }
  }

  return PR_DECLINED(cmd);
}
