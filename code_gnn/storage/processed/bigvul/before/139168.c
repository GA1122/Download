size_t RenderProcessHostImpl::GetWorkerRefCount() const {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  return service_worker_ref_count_ + shared_worker_ref_count_;
}
