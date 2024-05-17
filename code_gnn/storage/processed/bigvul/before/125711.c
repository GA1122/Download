void RenderViewHostImpl::SetNavigationsSuspended(bool suspend) {
  DCHECK(navigations_suspended_ != suspend);

  navigations_suspended_ = suspend;
  if (!suspend && suspended_nav_message_.get()) {
    SetSwappedOut(false);

    Send(suspended_nav_message_.release());
  }
}
