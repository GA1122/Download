frobnicate_signal_handler_2 (DBusGProxy  *proxy,
			     int          val,
			     void        *user_data)
{
  n_times_frobnicate_received_2 += 1;

  g_assert (val == 42);
  g_print ("Got Frobnicate signal (again)\n");
}
