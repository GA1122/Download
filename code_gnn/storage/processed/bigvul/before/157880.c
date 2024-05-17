void WebContentsImpl::UpdateUserGestureCarryoverInfo() {
  if (delegate_)
    delegate_->UpdateUserGestureCarryoverInfo(this);
}
