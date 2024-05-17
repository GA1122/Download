void WebContentsImpl::OnIgnoredUIEvent() {
  for (auto& observer : observers_)
    observer.DidGetIgnoredUIEvent();
}
