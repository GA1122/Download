void RenderViewHostImpl::DesktopNotificationPostDisplay(int callback_context) {
  Send(new DesktopNotificationMsg_PostDisplay(GetRoutingID(),
                                              callback_context));
}
