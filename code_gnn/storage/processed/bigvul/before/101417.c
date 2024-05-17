  void Observe(int type,
      const NotificationSource& source,
      const NotificationDetails& details) {
    switch (type) {
      case chrome::NOTIFICATION_FOREIGN_SESSION_UPDATED:
        notified_of_update_ = true;
        break;
      default:
        NOTREACHED();
        break;
    }
  }
