bool GLES2DecoderImpl::RegenerateRenderbufferIfNeeded(
    Renderbuffer* renderbuffer) {
  if (!renderbuffer->RegenerateAndBindBackingObjectIfNeeded(workarounds())) {
    return false;
  }

  if (renderbuffer != state_.bound_renderbuffer.get()) {
    state_.bound_renderbuffer_valid = false;
  }

  return true;
}
