scoped_refptr<base::SingleThreadTaskRunner> FallbackToInternalIfNull(
    const scoped_refptr<base::SingleThreadTaskRunner>& cache_thread) {
  return cache_thread ? cache_thread : InternalCacheThread();
}
