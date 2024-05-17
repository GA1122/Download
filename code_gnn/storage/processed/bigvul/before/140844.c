void GLES2DecoderImpl::OnContextLostError() {
  group_->LoseContexts(GL_UNKNOWN_CONTEXT_RESET_ARB);
}
