mojom::FrameHost* RenderFrameImpl::GetFrameHost() {
  if (!frame_host_ptr_.is_bound())
    GetRemoteAssociatedInterfaces()->GetInterface(&frame_host_ptr_);
  return frame_host_ptr_.get();
}
