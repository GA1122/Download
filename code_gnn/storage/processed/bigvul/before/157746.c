void WebContentsImpl::DecrementCapturerCount() {
  --capturer_count_;
  DVLOG(1) << "There are now " << capturer_count_
           << " capturing(s) of WebContentsImpl@" << this;
  DCHECK_LE(0, capturer_count_);

  if (is_being_destroyed_)
    return;

  if (!IsBeingCaptured()) {
    const gfx::Size old_size = preferred_size_for_capture_;
    preferred_size_for_capture_ = gfx::Size();
    OnPreferredSizeChanged(old_size);

    if (visibility_ == Visibility::HIDDEN) {
      DVLOG(1) << "Executing delayed WasHidden().";
      WasHidden();
    } else if (visibility_ == Visibility::OCCLUDED) {
      WasOccluded();
    }
  }
}
