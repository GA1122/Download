SystemKeyEventListener* SystemKeyEventListener::GetInstance() {
  VLOG_IF(1, !g_system_key_event_listener)
      << "SystemKeyEventListener::GetInstance() with NULL global instance.";
  return g_system_key_event_listener;
}
