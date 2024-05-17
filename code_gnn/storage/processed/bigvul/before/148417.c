device::mojom::WakeLockContext* WebContentsImpl::GetWakeLockContext() {
  if (!wake_lock_context_host_)
    wake_lock_context_host_.reset(new WakeLockContextHost(this));
  return wake_lock_context_host_->GetWakeLockContext();
}
