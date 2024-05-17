void NavigationControllerImpl::ContinuePendingReload() {
  if (pending_reload_ == ReloadType::NONE) {
    NOTREACHED();
  } else {
    Reload(pending_reload_, false);
    pending_reload_ = ReloadType::NONE;
  }
}
