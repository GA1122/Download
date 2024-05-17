bool GLES2DecoderImpl::HasMoreIdleWork() const {
  return !pending_readpixel_fences_.empty() ||
         gpu_tracer_->HasTracesToProcess() ||
         !texture_refs_pending_destruction_.empty();
}
