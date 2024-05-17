increment_async_cb (DBusGProxy *proxy, guint val, GError *error, gpointer data)
{
  if (error)
    lose_gerror ("Failed to complete (wrapped async) Increment call", error);

  if (data != NULL)
    lose ("(wrapped async) Increment call gave unexpected data");
  if (val != 43)
    lose ("(wrapped async) Increment call returned %d, should be 43", val);

  g_print ("(wrapped async) increment gave \"%d\"\n", val); 
  g_main_loop_quit (loop);
   g_source_remove (exit_timeout);
 }
