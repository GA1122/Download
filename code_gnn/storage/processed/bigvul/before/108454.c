void ChromeRenderProcessObserver::OnPurgeMemory() {
  RenderThread::Get()->EnsureWebKitInitialized();

  WebCache::clear();

  WebFontCache::clear();

  WebCrossOriginPreflightResultCache::clear();

  while (sqlite3_release_memory(std::numeric_limits<int>::max()) > 0) {
  }

  v8::V8::LowMemoryNotification();

  base::allocator::ReleaseFreeMemory();

  if (client_)
    client_->OnPurgeMemory();
}
