on_file_monitor_changed (GFileMonitor     *file_monitor,
                         GFile            *file,
                         GFile            *other_file,
                         GFileMonitorEvent event_type,
                         gpointer          user_data)
{
  PolkitBackendSessionMonitor *monitor = POLKIT_BACKEND_SESSION_MONITOR (user_data);

   
  if (monitor->database != NULL)
    {
      g_key_file_free (monitor->database);
      monitor->database = NULL;
    }
  g_signal_emit (monitor, signals[CHANGED_SIGNAL], 0);
}
