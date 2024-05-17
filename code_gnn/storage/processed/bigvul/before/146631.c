bool DrawingBuffer::PaintRenderingResultsToImageData(
    int& width,
    int& height,
    SourceDrawingBuffer source_buffer,
    WTF::ArrayBufferContents& contents) {
  ScopedStateRestorer scoped_state_restorer(this);

  DCHECK(!premultiplied_alpha_);
  width = Size().Width();
  height = Size().Height();

  CheckedNumeric<int> data_size = 4;
  data_size *= width;
  data_size *= height;
  if (!data_size.IsValid())
    return false;

  WTF::ArrayBufferContents pixels(width * height, 4,
                                  WTF::ArrayBufferContents::kNotShared,
                                  WTF::ArrayBufferContents::kDontInitialize);

  GLuint fbo = 0;
  state_restorer_->SetFramebufferBindingDirty();
  if (source_buffer == kFrontBuffer && front_color_buffer_) {
    gl_->GenFramebuffers(1, &fbo);
    gl_->BindFramebuffer(GL_FRAMEBUFFER, fbo);
    gl_->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              front_color_buffer_->parameters.target,
                              front_color_buffer_->texture_id, 0);
  } else {
    gl_->BindFramebuffer(GL_FRAMEBUFFER, fbo_);
  }

  ReadBackFramebuffer(static_cast<unsigned char*>(pixels.Data()), width, height,
                      kReadbackRGBA, WebGLImageConversion::kAlphaDoNothing);
  FlipVertically(static_cast<uint8_t*>(pixels.Data()), width, height);

  if (fbo) {
    gl_->FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              front_color_buffer_->parameters.target, 0, 0);
    gl_->DeleteFramebuffers(1, &fbo);
  }

  pixels.Transfer(contents);
  return true;
}
