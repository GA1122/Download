void WebContentsImpl::AddCreatedCallback(const CreatedCallback& callback) {
  g_created_callbacks.Get().push_back(callback);
}
