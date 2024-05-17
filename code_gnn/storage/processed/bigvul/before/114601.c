void RenderThreadImpl::PostponeIdleNotification() {
  idle_notifications_to_skip_ = 2;
}
