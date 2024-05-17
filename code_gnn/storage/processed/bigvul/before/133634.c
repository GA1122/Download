void WebContentsImpl::Stop() {
  GetRenderManager()->Stop();
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, NavigationStopped());
}
