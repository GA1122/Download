void RenderView::OnAccessibilityNotificationsAck() {
  DCHECK(accessibility_ack_pending_);
  accessibility_ack_pending_ = false;
  SendPendingAccessibilityNotifications();
}
