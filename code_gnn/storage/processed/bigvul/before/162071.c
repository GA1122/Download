void RenderProcessHostImpl::DecrementKeepAliveRefCount() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!is_keep_alive_ref_count_disabled_);
  DCHECK_GT(keep_alive_ref_count_, 0U);
  --keep_alive_ref_count_;
  if (keep_alive_ref_count_ == 0)
    Cleanup();
}
