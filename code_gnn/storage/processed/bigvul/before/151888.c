void RenderFrameHostImpl::BindWakeLockRequest(
    device::mojom::WakeLockRequest request) {
  device::mojom::WakeLock* renderer_wake_lock =
      delegate_ ? delegate_->GetRendererWakeLock() : nullptr;
  if (renderer_wake_lock)
    renderer_wake_lock->AddClient(std::move(request));
}
