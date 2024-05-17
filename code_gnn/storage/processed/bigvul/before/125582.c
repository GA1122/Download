void RenderViewHostImpl::DesktopNotificationPostError(int notification_id,
                                                      const string16& message) {
  Send(new DesktopNotificationMsg_PostError(
      GetRoutingID(), notification_id, message));
}
