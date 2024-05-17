void GLES2DecoderImpl::DoLoseContextCHROMIUM(GLenum current, GLenum other) {
  group_->LoseContexts(other);
  reset_status_ = current;
  current_decoder_error_ = error::kLostContext;
}
