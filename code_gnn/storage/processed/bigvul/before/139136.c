void RenderProcessHostImpl::DecrementSharedWorkerRefCount() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!is_worker_ref_count_disabled_);
  DCHECK_GT(GetWorkerRefCount(), 0U);
  --shared_worker_ref_count_;
  if (GetWorkerRefCount() == 0)
    Cleanup();
}
