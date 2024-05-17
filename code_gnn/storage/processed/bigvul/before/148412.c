device::mojom::WakeLock* WebContentsImpl::GetRendererWakeLock() {
  if (!renderer_wake_lock_) {
    device::mojom::WakeLockContext* wake_lock_context = GetWakeLockContext();
    if (!wake_lock_context) {
      return nullptr;
    }
    wake_lock_context->GetWakeLock(
        device::mojom::WakeLockType::PreventDisplaySleep,
        device::mojom::WakeLockReason::ReasonOther, "Wake Lock API",
        mojo::MakeRequest(&renderer_wake_lock_));
  }
  return renderer_wake_lock_.get();
}
