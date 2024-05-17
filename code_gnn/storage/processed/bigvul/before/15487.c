invalidate_persistent (void)
{
  DEBUGP (("Disabling further reuse of socket %d.\n", pconn.socket));
  pconn_active = false;
  fd_close (pconn.socket);
  xfree (pconn.host);
  xzero (pconn);
}
