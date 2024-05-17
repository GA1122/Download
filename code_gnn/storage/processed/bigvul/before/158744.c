ScopedFramebufferBinder::ScopedFramebufferBinder(GLES2DecoderImpl* decoder,
                                                 GLuint id)
    : decoder_(decoder) {
  ScopedGLErrorSuppressor suppressor(
      "ScopedFramebufferBinder::ctor", decoder_->GetErrorState());
  decoder->api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, id);
  decoder->OnFboChanged();
}
