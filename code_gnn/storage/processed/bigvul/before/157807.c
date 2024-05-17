void WebContentsImpl::MediaMutedStatusChanged(
    const WebContentsObserver::MediaPlayerId& id,
    bool muted) {
  for (auto& observer : observers_)
    observer.MediaMutedStatusChanged(id, muted);
}
