  explicit ScopedRedrawLock(HWNDMessageHandler* owner)
    : owner_(owner),
      hwnd_(owner_->hwnd()),
      was_visible_(owner_->IsVisible()),
      cancel_unlock_(false),
      force_(!(GetWindowLong(hwnd_, GWL_STYLE) & WS_CAPTION)) {
    if (was_visible_ && ::IsWindow(hwnd_))
      owner_->LockUpdates(force_);
  }
