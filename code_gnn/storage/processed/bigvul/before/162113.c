void RenderProcessHostImpl::OnAec3Enabled() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!aec3_set_callback_.is_null())
    std::move(aec3_set_callback_).Run(true, std::string());
}
