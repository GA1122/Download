void RenderThreadImpl::CompositingModeFallbackToSoftware() {
  if (gpu_channel_) {
    gpu_channel_->DestroyChannel();
    gpu_channel_ = nullptr;
  }

  is_gpu_compositing_disabled_ = true;
}
