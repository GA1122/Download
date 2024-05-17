void RenderViewHostImpl::OnRequestDesktopNotificationPermission(
    const GURL& source_origin, int callback_context) {
  GetContentClient()->browser()->RequestDesktopNotificationPermission(
      source_origin, callback_context, GetProcess()->GetID(), GetRoutingID());
}
