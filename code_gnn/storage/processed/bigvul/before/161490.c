TargetHandler::Throttle::Throttle(
    base::WeakPtr<protocol::TargetHandler> target_handler,
    content::NavigationHandle* navigation_handle)
    : content::NavigationThrottle(navigation_handle),
      target_handler_(target_handler) {
  target_handler->throttles_.insert(this);
}
