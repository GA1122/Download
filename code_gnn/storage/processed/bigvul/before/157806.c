void WebContentsImpl::MediaEffectivelyFullscreenChanged(bool is_fullscreen) {
  for (auto& observer : observers_)
    observer.MediaEffectivelyFullscreenChanged(is_fullscreen);
}
