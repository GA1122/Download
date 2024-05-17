bool WebFrameLoaderClient::ActionSpecifiesNavigationPolicy(
    const WebCore::NavigationAction& action,
    WebNavigationPolicy* policy) {
  if ((action.type() != NavigationTypeLinkClicked) ||
      !action.event()->isMouseEvent())
    return false;

  const MouseEvent* event = static_cast<const MouseEvent*>(action.event());
  return WebViewImpl::NavigationPolicyFromMouseEvent(event->button(),
      event->ctrlKey(), event->shiftKey(), event->altKey(), event->metaKey(),
      policy);
}
