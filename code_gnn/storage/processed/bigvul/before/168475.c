void CoordinatorImpl::RegisterHeapProfiler(
    mojom::HeapProfilerPtr heap_profiler) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  heap_profiler_ = std::move(heap_profiler);
}
