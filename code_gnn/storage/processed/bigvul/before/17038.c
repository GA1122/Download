QEvent::Type GetPrepareToCloseBypassEventType() {
  static int g_event_type = QEvent::registerEventType();
  return QEvent::Type(g_event_type);
}
