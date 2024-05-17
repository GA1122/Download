  explicit PrunedListener(NavigationController* controller)
      : notification_count_(0) {
    registrar_.Add(this, content::NOTIFICATION_NAV_LIST_PRUNED,
                   content::Source<NavigationController>(controller));
  }
