void WebContentsImpl::NotifyNavigationStateChanged(
    InvalidateTypes changed_flags) {
  if (changed_flags & INVALIDATE_TYPE_TAB) {
    media_web_contents_observer_->MaybeUpdateAudibleState();
  }

  if (delegate_)
    delegate_->NavigationStateChanged(this, changed_flags);

  if (GetOuterWebContents())
    GetOuterWebContents()->NotifyNavigationStateChanged(changed_flags);
}
