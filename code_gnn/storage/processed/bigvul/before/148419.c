bool WebContentsImpl::GotResponseToLockMouseRequest(bool allowed) {
  if (!GuestMode::IsCrossProcessFrameGuest(GetWebContents()) &&
      GetBrowserPluginGuest())
    return GetBrowserPluginGuest()->LockMouse(allowed);

  if (mouse_lock_widget_) {
    if (mouse_lock_widget_->delegate()->GetAsWebContents() != this) {
      return mouse_lock_widget_->delegate()
          ->GetAsWebContents()
          ->GotResponseToLockMouseRequest(allowed);
    }

    if (mouse_lock_widget_->GotResponseToLockMouseRequest(allowed))
      return true;
  }

  for (WebContentsImpl* current = this; current;
       current = current->GetOuterWebContents()) {
    current->mouse_lock_widget_ = nullptr;
  }

  return false;
}
