void RenderFrameImpl::requestNotificationPermission(
    const blink::WebSecurityOrigin& origin,
    blink::WebNotificationPermissionCallback* callback) {
  if (!notification_permission_dispatcher_) {
    notification_permission_dispatcher_ =
        new NotificationPermissionDispatcher(this);
  }

  notification_permission_dispatcher_->RequestPermission(origin, callback);
}
