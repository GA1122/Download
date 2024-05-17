ScopedFrameBufferBinder::ScopedFrameBufferBinder(GLES2DecoderImpl* decoder,
                                                 GLuint id)
    : decoder_(decoder) {
  ScopedGLErrorSuppressor suppressor(
      "ScopedFrameBufferBinder::ctor", decoder_->GetErrorState());
  glBindFramebufferEXT(GL_FRAMEBUFFER, id);
  decoder->OnFboChanged();
}
