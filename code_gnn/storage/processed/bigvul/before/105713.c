  void Observe(NotificationType type,
               const NotificationSource& source,
               const NotificationDetails& details) {
    if (type.value == NotificationType::APP_TERMINATING) {
      notification_registrar_.RemoveAll();
      StopInputMethodDaemon();
#if !defined(TOUCH_UI)
      candidate_window_controller_.reset(NULL);
#endif
    }
  }
