  bool GrabDeferredLock() {
    if (root_window_ && defer_compositor_lock_) {
      compositor_lock_ = root_window_->compositor()->GetCompositorLock();
      defer_compositor_lock_ = false;
      return true;
    }
    return false;
  }
