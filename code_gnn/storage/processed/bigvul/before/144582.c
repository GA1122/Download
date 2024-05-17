void WebContentsImpl::MediaStoppedPlaying(
    const WebContentsObserver::MediaPlayerId& id) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, MediaStoppedPlaying(id));
}
