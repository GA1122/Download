  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) {
    if (type == content::NOTIFICATION_NAV_LIST_PRUNED) {
      notification_count_++;
      details_ = *(content::Details<content::PrunedDetails>(details).ptr());
    }
  }
