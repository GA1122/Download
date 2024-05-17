  ~ScopedRedrawLock() {
    if (!cancel_unlock_ && was_visible_ && ::IsWindow(hwnd_))
      owner_->UnlockUpdates(force_);
  }
