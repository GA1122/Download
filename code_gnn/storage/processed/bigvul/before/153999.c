void GLES2DecoderImpl::DoFlushDriverCachesCHROMIUM(void) {
  if (workarounds().unbind_egl_context_to_flush_driver_caches) {
    context_->ReleaseCurrent(nullptr);
    context_->MakeCurrent(surface_.get());
  }
}
