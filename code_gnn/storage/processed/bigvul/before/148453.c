void WebContentsImpl::NotifyNavigationStateChanged(
    InvalidateTypes changed_flags) {
  tracked_objects::ScopedTracker tracking_profile(
      FROM_HERE_WITH_EXPLICIT_FUNCTION(
          "466285 WebContentsImpl::NotifyNavigationStateChanged"));
  if (changed_flags & INVALIDATE_TYPE_TAB) {
    media_web_contents_observer_->MaybeUpdateAudibleState();
  }

  if (delegate_)
    delegate_->NavigationStateChanged(this, changed_flags);

  if (GetOuterWebContents())
    GetOuterWebContents()->NotifyNavigationStateChanged(changed_flags);
}
