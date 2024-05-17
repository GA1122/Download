void RenderThreadImpl::OnMemoryStateChange(base::MemoryState state) {
  if (blink_platform_impl_) {
    blink::WebMemoryCoordinator::OnMemoryStateChange(
        static_cast<blink::MemoryState>(state));
  }
}
