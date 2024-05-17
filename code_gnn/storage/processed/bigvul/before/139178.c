bool RenderProcessHostImpl::IsWorkerRefCountDisabled() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  return is_worker_ref_count_disabled_;
}
