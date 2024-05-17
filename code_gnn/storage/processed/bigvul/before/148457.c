void WebContentsImpl::NotifyWebContentsFocused() {
  for (auto& observer : observers_)
    observer.OnWebContentsFocused();
}
