bool GLES2DecoderImpl::VerifyMultisampleRenderbufferIntegrity(
    GLuint renderbuffer, GLenum format) {

  GLenum pixel_format = GL_RGBA;
  GLenum pixel_type = GL_UNSIGNED_BYTE;
  switch (format) {
    case GL_RGB8:
      pixel_format = GL_RGB;
      break;
    case GL_RGBA8:
      break;
    default:
      return true;
  }

  GLint draw_framebuffer, read_framebuffer;

  api()->glGetIntegervFn(GL_DRAW_FRAMEBUFFER_BINDING, &draw_framebuffer);
  api()->glGetIntegervFn(GL_READ_FRAMEBUFFER_BINDING, &read_framebuffer);

  if (!validation_fbo_) {
    api()->glGenFramebuffersEXTFn(1, &validation_fbo_multisample_);
    api()->glGenFramebuffersEXTFn(1, &validation_fbo_);
  }

  GLint bound_texture;
  api()->glGetIntegervFn(GL_TEXTURE_BINDING_2D, &bound_texture);
  GLuint validation_texture;
  TextureMap::iterator iter = validation_textures_.find(format);
  if (iter == validation_textures_.end()) {
    api()->glGenTexturesFn(1, &validation_texture);
    validation_textures_.insert(std::make_pair(format, validation_texture));

    api()->glBindTextureFn(GL_TEXTURE_2D, validation_texture);
    api()->glTexImage2DFn(GL_TEXTURE_2D, 0, format, 1, 1, 0, pixel_format,
                          pixel_type, nullptr);
  } else {
    validation_texture = iter->second;
  }
  api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, validation_fbo_);
  api()->glFramebufferTexture2DEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                     GL_TEXTURE_2D, validation_texture, 0);
  api()->glBindTextureFn(GL_TEXTURE_2D, bound_texture);

  api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, validation_fbo_multisample_);
  api()->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                        GL_RENDERBUFFER, renderbuffer);

  GLboolean scissor_enabled = false;
  api()->glGetBooleanvFn(GL_SCISSOR_TEST, &scissor_enabled);
  if (scissor_enabled)
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);
  ClearDeviceWindowRectangles();

  GLboolean color_mask[4] = {GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE};
  api()->glGetBooleanvFn(GL_COLOR_WRITEMASK, color_mask);
  state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  GLfloat clear_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  api()->glGetFloatvFn(GL_COLOR_CLEAR_VALUE, clear_color);
  api()->glClearColorFn(1.0f, 0.0f, 1.0f, 1.0f);

  api()->glClearFn(GL_COLOR_BUFFER_BIT);

  api()->glBindFramebufferEXTFn(GL_READ_FRAMEBUFFER,
                                validation_fbo_multisample_);
  api()->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER, validation_fbo_);

  api()->glBlitFramebufferFn(0, 0, 1, 1, 0, 0, 1, 1, GL_COLOR_BUFFER_BIT,
                             GL_NEAREST);

  api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, validation_fbo_);

  unsigned char pixel[3] = {0, 0, 0};
  api()->glReadPixelsFn(0, 0, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

  api()->glBindFramebufferEXTFn(GL_FRAMEBUFFER, validation_fbo_multisample_);
  api()->glFramebufferRenderbufferEXTFn(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                        GL_RENDERBUFFER, 0);

  if (scissor_enabled)
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, true);
  RestoreDeviceWindowRectangles();

  state_.SetDeviceColorMask(
      color_mask[0], color_mask[1], color_mask[2], color_mask[3]);
  api()->glClearColorFn(clear_color[0], clear_color[1], clear_color[2],
                        clear_color[3]);
  api()->glBindFramebufferEXTFn(GL_DRAW_FRAMEBUFFER, draw_framebuffer);
  api()->glBindFramebufferEXTFn(GL_READ_FRAMEBUFFER, read_framebuffer);

  return (pixel[0] == 0xFF &&
      pixel[1] == 0x00 &&
      pixel[2] == 0xFF);
}
