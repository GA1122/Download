  void Observe(int type,
      const content::NotificationSource& source,
      const content::NotificationDetails& details) {
    switch (type) {
      case chrome::NOTIFICATION_FOREIGN_SESSION_UPDATED:
        notified_of_update_ = true;
        break;
      case chrome::NOTIFICATION_SYNC_REFRESH:
        notified_of_refresh_ = true;
        break;
      default:
        NOTREACHED();
        break;
    }
  }
