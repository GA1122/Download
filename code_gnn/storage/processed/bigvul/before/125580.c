void RenderViewHostImpl::DesktopNotificationPostClose(int notification_id,
                                                      bool by_user) {
  Send(new DesktopNotificationMsg_PostClose(
      GetRoutingID(), notification_id, by_user));
}
