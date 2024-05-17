bool AutofillDialogViews::NotificationArea::HasArrow() {
  return !notifications_.empty() && notifications_[0].HasArrow() &&
      arrow_centering_anchor_.get();
}
