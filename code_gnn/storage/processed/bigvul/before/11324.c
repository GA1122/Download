increment_received_cb (DBusGProxy *proxy,
		       DBusGProxyCall *call,
		       gpointer data)
{
  GError *error;
  guint val;

  g_assert (!strcmp (data, "moo"));

  error = NULL;
  if (!dbus_g_proxy_end_call (proxy, call, &error,
			      G_TYPE_UINT, &val,
			      G_TYPE_INVALID))
    lose_gerror ("Failed to complete (async) Increment call", error);

  if (val != 43)
    lose ("Increment call returned %d, should be 43", val);
  
  g_print ("Async increment gave \"%d\"\n", val); 
  g_main_loop_quit (loop);
  g_source_remove (exit_timeout);
}
