void TabAppendedNotificationObserver::ObserveTab(
    NavigationController* controller) {
  if (!automation_ || !reply_message_.get())
    return;

  if (automation_->GetIndexForNavigationController(controller, parent_) ==
      TabStripModel::kNoTab) {
    return;
  }

  new NavigationNotificationObserver(controller, automation_,
                                     reply_message_.release(),
                                     1, false, use_json_interface_);
}
