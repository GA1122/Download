void WebContentsImpl::MediaResized(
    const gfx::Size& size,
    const WebContentsObserver::MediaPlayerId& id) {
  cached_video_sizes_[id] = size;

  for (auto& observer : observers_)
    observer.MediaResized(size, id);
}
