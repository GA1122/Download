void ResourceDispatcherHostImpl::OnUserGesture(WebContentsImpl* contents) {
  last_user_gesture_time_ = TimeTicks::Now();
}
