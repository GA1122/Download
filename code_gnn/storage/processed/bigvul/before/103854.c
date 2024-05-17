void RenderView::SendPendingAccessibilityNotifications() {
  if (!accessibility_.get())
    return;

  if (pending_accessibility_notifications_.empty())
    return;

  std::vector<ViewHostMsg_AccessibilityNotification_Params> notifications;
  for (size_t i = 0; i < pending_accessibility_notifications_.size(); i++) {
    RendererAccessibilityNotification& notification =
        pending_accessibility_notifications_[i];
    WebAccessibilityObject obj = accessibility_->getObjectById(notification.id);
    if (!obj.isValid())
      continue;

    ViewHostMsg_AccessibilityNotification_Params param;
    WebAccessibilityNotificationToViewHostMsg(
        pending_accessibility_notifications_[i].type, &param.notification_type);
    param.acc_obj = WebAccessibility(
        obj, accessibility_.get(), notification.ShouldIncludeChildren());
    notifications.push_back(param);
  }
  pending_accessibility_notifications_.clear();
  Send(new ViewHostMsg_AccessibilityNotifications(routing_id_, notifications));
  accessibility_ack_pending_ = true;
}
