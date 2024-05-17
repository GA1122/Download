echo_received_cb (DBusGProxy *proxy,
		  DBusGProxyCall *call,
		  gpointer data)
{
  GError *error;
  char *echo_data;

  g_assert (call == echo_call);
  g_assert (data == NULL);

  error = NULL;
  echo_data = NULL;
  n_times_echo_cb_entered++;

  if (!dbus_g_proxy_end_call (proxy, call, &error,
			      G_TYPE_STRING,
			      &echo_data,
			      G_TYPE_INVALID))
    lose_gerror ("Failed to complete async Echo", error);
  g_assert (echo_data != NULL);
  g_print ("Async echo gave \"%s\"\n", echo_data); 
  g_free (echo_data);
  g_main_loop_quit (loop);
  g_source_remove (exit_timeout);
}
