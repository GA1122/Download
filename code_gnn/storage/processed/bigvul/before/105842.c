void UrlmonUrlRequest::ReleaseBindings() {
  binding_.Release();
  if (bind_context_)
    ::RevokeBindStatusCallback(bind_context_, this);
}
