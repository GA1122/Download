void RenderViewHostImpl::DesktopNotificationPermissionRequestDone(
    int callback_context) {
  Send(new DesktopNotificationMsg_PermissionRequestDone(
      GetRoutingID(), callback_context));
}
