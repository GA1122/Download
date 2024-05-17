base::TaskRunner* RenderThreadImpl::GetWorkerTaskRunner() {
  return categorized_worker_pool_.get();
}
