  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details) {
    TabContentsWrapper* tab_to_delete = tab_to_delete_;
    tab_to_delete_ = NULL;
    delete tab_to_delete;
  }
