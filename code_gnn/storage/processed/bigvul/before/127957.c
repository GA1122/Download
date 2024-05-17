void BrowserViewRenderer::ReturnResourceFromParent() {
  cc::CompositorFrameAck frame_ack;
  shared_renderer_state_.SwapReturnedResourcesOnUI(&frame_ack.resources);
  if (compositor_ && !frame_ack.resources.empty()) {
    compositor_->ReturnResources(frame_ack);
  }
}
