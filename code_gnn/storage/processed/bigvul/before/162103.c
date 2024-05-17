void RenderProcessHostImpl::IncrementKeepAliveRefCount() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!is_keep_alive_ref_count_disabled_);
  ++keep_alive_ref_count_;
}
