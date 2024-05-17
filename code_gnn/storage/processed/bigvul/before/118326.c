void AutofillDialogViews::UpdateNotificationArea() {
  DCHECK(notification_area_);
  notification_area_->SetNotifications(delegate_->CurrentNotifications());
  ContentsPreferredSizeChanged();
}
