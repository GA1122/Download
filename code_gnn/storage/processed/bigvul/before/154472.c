bool GLES2DecoderPassthroughImpl::HasMoreIdleWork() const {
  return gpu_tracer_->HasTracesToProcess() || !pending_read_pixels_.empty() ||
         resources_->HasTexturesPendingDestruction();
}
