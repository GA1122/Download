void RenderThreadImpl::PendingFrameCreate::OnConnectionError() {
  size_t erased =
      RenderThreadImpl::current()->pending_frame_creates_.erase(routing_id_);
  DCHECK_EQ(1u, erased);
}
