void RenderFrameHostImpl::GetCanonicalUrlForSharing(
    mojom::Frame::GetCanonicalUrlForSharingCallback callback) {
  DCHECK(frame_);
  frame_->GetCanonicalUrlForSharing(std::move(callback));
}
