void AutofillDialogViews::NotificationArea::SetNotifications(
    const std::vector<DialogNotification>& notifications) {
  notifications_ = notifications;

  RemoveAllChildViews(true);

  if (notifications_.empty())
    return;

  for (size_t i = 0; i < notifications_.size(); ++i) {
    const DialogNotification& notification = notifications_[i];
    scoped_ptr<NotificationView> view(new NotificationView(notification,
                                                           delegate_));

    AddChildView(view.release());
  }

  PreferredSizeChanged();
}
