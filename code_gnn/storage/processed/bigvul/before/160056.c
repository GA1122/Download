scoped_refptr<base::SingleThreadTaskRunner> InternalCacheThread() {
  return g_internal_cache_thread.Get().task_runner();
}
