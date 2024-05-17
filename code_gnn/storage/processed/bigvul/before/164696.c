void RenderFrameHostImpl::OnPortalActivated(blink::TransferableMessage data) {
  frame_->OnPortalActivated(std::move(data));
}
