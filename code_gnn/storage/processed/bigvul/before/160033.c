void BackendImpl::FlushForTesting() {
  g_internal_cache_thread.Get().FlushForTesting();
}
