bool GLES2DecoderImpl::DoIsEnabled(GLenum cap) {
  return state_.GetEnabled(cap);
}
