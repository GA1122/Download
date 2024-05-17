void WebContentsImpl::MediaStartedPlaying(
    const WebContentsObserver::MediaPlayerId& id) {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, MediaStartedPlaying(id));
}
