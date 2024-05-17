void CoordinatorImpl::BindHeapProfilerHelperRequest(
    mojom::HeapProfilerHelperRequest request,
    const service_manager::BindSourceInfo& source_info) {
  DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
  bindings_heap_profiler_helper_.AddBinding(this, std::move(request),
                                            source_info.identity);
}
