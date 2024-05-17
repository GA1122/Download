void WebContentsImpl::RemoveDestructionObserver(WebContentsImpl* web_contents) {
  destruction_observers_.erase(web_contents);
}
