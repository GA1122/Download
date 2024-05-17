void RenderViewHostImpl::CancelSuspendedNavigations() {
  if (suspended_nav_message_.get())
    suspended_nav_message_.reset();
  navigations_suspended_ = false;
}
