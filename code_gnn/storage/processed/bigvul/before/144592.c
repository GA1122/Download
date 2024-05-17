void WebContentsImpl::NotifyWebContentsFocused() {
  FOR_EACH_OBSERVER(WebContentsObserver, observers_, OnWebContentsFocused());
}
