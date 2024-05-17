void WebContentsImpl::IncrementCapturerCount(const gfx::Size& capture_size) {
  DCHECK(!is_being_destroyed_);
  ++capturer_count_;
  DVLOG(1) << "There are now " << capturer_count_
           << " capturing(s) of WebContentsImpl@" << this;

  if (!capture_size.IsEmpty() && preferred_size_for_capture_.IsEmpty()) {
    preferred_size_for_capture_ = capture_size;
    OnPreferredSizeChanged(preferred_size_);
  }

  DoWasUnOccluded();
}