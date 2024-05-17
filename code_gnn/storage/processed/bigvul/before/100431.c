void RenderThread::OnPurgeMemory() {
  EnsureWebKitInitialized();

  WebCache::clear();

  WebFontCache::clear();

  WebCrossOriginPreflightResultCache::clear();

  while (!v8::V8::IdleNotification())
    ;

#if defined(OS_WIN)
  MallocExtension::instance()->ReleaseFreeMemory();
#endif
}
