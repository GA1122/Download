static void login_succeeded(pool *p, const char *user) {
#ifdef HAVE_LOGINSUCCESS
  const char *host, *sess_ttyname;
  char *msg = NULL;
  int res, xerrno;

  host = pr_netaddr_get_dnsstr(session.c->remote_addr);
  sess_ttyname = pr_session_get_ttyname(p);

  PRIVS_ROOT
  res = loginsuccess((char *) user, (char *) host, (char *) sess_ttyname, &msg);
  xerrno = errno;
  PRIVS_RELINQUISH

  if (res == 0) {
    if (msg != NULL) {
      pr_trace_msg("auth", 14, "AIX loginsuccess() report: %s", msg);
    }

  } else {
    pr_trace_msg("auth", 3, "AIX loginsuccess() error for user '%s', "
      "host '%s', tty '%s': %s", user, host, sess_ttyname, strerror(errno));
  }

  if (msg != NULL) {
    free(msg);
  }
#endif  
}
