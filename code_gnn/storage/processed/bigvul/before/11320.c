foo_signal_handler (DBusGProxy  *proxy,
                    double       d,
                    void        *user_data)
{
  n_times_foo_received += 1;

  g_print ("Got Foo signal\n");

  g_main_loop_quit (loop);
  g_source_remove (exit_timeout);
}
