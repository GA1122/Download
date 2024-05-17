void RenderViewHostImpl::OnCancelDesktopNotification(int notification_id) {
  GetContentClient()->browser()->CancelDesktopNotification(
      GetProcess()->GetID(), GetRoutingID(), notification_id);
}
