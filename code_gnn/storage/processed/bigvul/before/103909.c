void RenderView::postAccessibilityNotification(
    const WebAccessibilityObject& obj,
    WebAccessibilityNotification notification) {
  if (!accessibility_.get() && webview()) {
    if (notification != WebKit::WebAccessibilityNotificationLoadComplete)
      return;

    accessibility_.reset(WebAccessibilityCache::create());
    accessibility_->initialize(webview());
  }

  if (!accessibility_->isCached(obj)) {
    WebAccessibilityObject parent = obj;
    while (parent.isValid() && !accessibility_->isCached(parent))
      parent = parent.parentObject();

    DCHECK(parent.isValid() && accessibility_->isCached(parent));
    if (!parent.isValid())
      return;
    postAccessibilityNotification(
        parent, WebKit::WebAccessibilityNotificationChildrenChanged);

    if (notification == WebKit::WebAccessibilityNotificationChildrenChanged)
      return;
  }

  RendererAccessibilityNotification acc_notification;
  acc_notification.id = accessibility_->addOrGetId(obj);
  acc_notification.type = notification;
  if (acc_notification.id < 0)
    return;

  ViewHostMsg_AccessibilityNotification_Type::Value temp;
  if (!WebAccessibilityNotificationToViewHostMsg(notification, &temp))
    return;

  for (uint32 i = 0; i < pending_accessibility_notifications_.size(); i++) {
    if (pending_accessibility_notifications_[i].id == acc_notification.id &&
        pending_accessibility_notifications_[i].type == acc_notification.type) {
      return;
    }
  }
  pending_accessibility_notifications_.push_back(acc_notification);

  if (!accessibility_ack_pending_ && accessibility_method_factory_.empty()) {
    MessageLoop::current()->PostTask(
        FROM_HERE,
        accessibility_method_factory_.NewRunnableMethod(
            &RenderView::SendPendingAccessibilityNotifications));
  }
}
