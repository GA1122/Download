static gboolean xt_event_prepare(GSource *source, gint *timeout)
{
  int mask = XtAppPending(x_app_context);
  if (mask)
	return TRUE;
   
  return (*timeout = xt_get_next_timeout(source)) == 0;
}
