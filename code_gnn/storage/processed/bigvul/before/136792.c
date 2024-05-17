ApplicationCache* LocalDOMWindow::applicationCache() const {
  if (!IsCurrentlyDisplayedInFrame())
    return nullptr;
  if (!application_cache_)
    application_cache_ = ApplicationCache::Create(GetFrame());
  return application_cache_.Get();
}
