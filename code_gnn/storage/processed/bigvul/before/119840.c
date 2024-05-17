  explicit PrunedListener(NavigationControllerImpl* controller)
      : notification_count_(0) {
    registrar_.Add(this, NOTIFICATION_NAV_LIST_PRUNED,
                   Source<NavigationController>(controller));
  }
