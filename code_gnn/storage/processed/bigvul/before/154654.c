error::Error GLES2DecoderPassthroughImpl::DoFlushDriverCachesCHROMIUM() {
  if (feature_info_->workarounds().unbind_egl_context_to_flush_driver_caches) {
    context_->ReleaseCurrent(nullptr);
    context_->MakeCurrent(surface_.get());
  }
  return error::kNoError;
}
