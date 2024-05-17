void RenderProcessHostImpl::ForceReleaseWorkerRefCounts() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!is_worker_ref_count_disabled_);
  is_worker_ref_count_disabled_ = true;
  if (!GetWorkerRefCount())
    return;
  service_worker_ref_count_ = 0;
  shared_worker_ref_count_ = 0;
  Cleanup();
}
