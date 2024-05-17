void WebContentsImpl::UpdatePreferredSize(const gfx::Size& pref_size) {
  const gfx::Size old_size = GetPreferredSize();
  preferred_size_ = pref_size;
  OnPreferredSizeChanged(old_size);
}
