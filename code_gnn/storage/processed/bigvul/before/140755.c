void GLES2DecoderImpl::DoSubscribeValueCHROMIUM(GLenum target,
                                                GLenum subscription) {
  if (!CheckCurrentValuebuffer("glSubscribeValueCHROMIUM")) {
    return;
  }
  state_.bound_valuebuffer.get()->AddSubscription(subscription);
}
