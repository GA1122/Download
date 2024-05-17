void NavigationControllerImpl::CancelPendingReload() {
  DCHECK(pending_reload_ != ReloadType::NONE);
  pending_reload_ = ReloadType::NONE;
}
