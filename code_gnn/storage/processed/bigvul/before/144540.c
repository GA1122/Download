gfx::Size WebContentsImpl::GetPreferredSize() const {
  return capturer_count_ == 0 ? preferred_size_ : preferred_size_for_capture_;
}
