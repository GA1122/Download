void GLES2DecoderImpl::DoPopulateSubscribedValuesCHROMIUM(GLenum target) {
  if (!CheckCurrentValuebuffer("glPopulateSubscribedValuesCHROMIUM")) {
    return;
  }
  valuebuffer_manager()->UpdateValuebufferState(state_.bound_valuebuffer.get());
}
