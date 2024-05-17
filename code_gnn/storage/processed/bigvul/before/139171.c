void RenderProcessHostImpl::IncrementSharedWorkerRefCount() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!is_worker_ref_count_disabled_);
  ++shared_worker_ref_count_;
}
