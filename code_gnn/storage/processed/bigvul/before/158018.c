void LocalFrameClientImpl::AnnotatedRegionsChanged() {
  web_frame_->Client()->DraggableRegionsChanged();
}
