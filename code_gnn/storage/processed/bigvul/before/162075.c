void RenderProcessHostImpl::DisableKeepAliveRefCount() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(!is_keep_alive_ref_count_disabled_);
  is_keep_alive_ref_count_disabled_ = true;
  if (!keep_alive_ref_count_)
    return;
  keep_alive_ref_count_ = 0;
  Cleanup();
}
