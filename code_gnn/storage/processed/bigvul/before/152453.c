void RenderFrameImpl::OnSetFrameOwnerProperties(
    const FrameOwnerProperties& frame_owner_properties) {
  DCHECK(frame_);
  frame_->SetFrameOwnerProperties(
      ConvertFrameOwnerPropertiesToWebFrameOwnerProperties(
          frame_owner_properties));
}
