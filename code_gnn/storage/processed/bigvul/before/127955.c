void BrowserViewRenderer::ReleaseHardware() {
  DCHECK(hardware_enabled_);
  ReturnUnusedResource(shared_renderer_state_.PassUncommittedFrameOnUI());
  ReturnResourceFromParent();
  DCHECK(shared_renderer_state_.ReturnedResourcesEmptyOnUI());

  if (compositor_) {
    compositor_->SetMemoryPolicy(0u);
  }

  hardware_enabled_ = false;
}
