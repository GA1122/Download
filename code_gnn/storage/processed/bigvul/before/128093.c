void SynchronousCompositorImpl::UpdateFrameMetaData(
    const cc::CompositorFrameMetadata& frame_metadata) {
  rwhva_->SynchronousFrameMetadata(frame_metadata);
  DeliverMessages();
}
