void ResourceDispatcherHostImpl::OnUserGesture() {
  last_user_gesture_time_ = TimeTicks::Now();
}
