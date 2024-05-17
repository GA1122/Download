bool RenderProcessHostImpl::IsKeepAliveRefCountDisabled() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  return is_keep_alive_ref_count_disabled_;
}
