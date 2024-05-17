void GLES2DecoderImpl::LoseContext(uint32 reset_status) {
  if (reset_status_ != GL_NO_ERROR) {
    return;
  }

  reset_status_ = reset_status;
  current_decoder_error_ = error::kLostContext;
}
