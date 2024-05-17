static int auth_login_timeout_cb(CALLBACK_FRAME) {
  pr_response_send_async(R_421,
    _("Login timeout (%d %s): closing control connection"), TimeoutLogin,
    TimeoutLogin != 1 ? "seconds" : "second");

   
  pr_log_pri(PR_LOG_INFO, "%s", "Login timeout exceeded, disconnected");
  pr_event_generate("core.timeout-login", NULL);

  pr_session_disconnect(&auth_module, PR_SESS_DISCONNECT_TIMEOUT,
    "TimeoutLogin");

   
  return 0;
}
