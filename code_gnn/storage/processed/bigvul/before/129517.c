bool GLES2DecoderImpl::VerifyMultisampleRenderbufferIntegrity(
    GLuint renderbuffer, GLenum format) {

  switch(format) {
    case GL_RGB:
    case GL_RGB8:
    case GL_RGBA:
    case GL_RGBA8:
      break;
    default:
      return true;
  }

  GLint draw_framebuffer, read_framebuffer;

  glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_framebuffer);
  glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_framebuffer);

  if (!validation_texture_) {
    GLint bound_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture);

    glGenTextures(1, &validation_texture_);
    glGenFramebuffersEXT(1, &validation_fbo_multisample_);
    glGenFramebuffersEXT(1, &validation_fbo_);

    glBindTexture(GL_TEXTURE_2D, validation_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB,
        GL_UNSIGNED_BYTE, NULL);

    glBindFramebufferEXT(GL_FRAMEBUFFER, validation_fbo_);
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D, validation_texture_, 0);

    glBindTexture(GL_TEXTURE_2D, bound_texture);
  }

  glBindFramebufferEXT(GL_FRAMEBUFFER, validation_fbo_multisample_);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_RENDERBUFFER, renderbuffer);

  GLboolean scissor_enabled = false;
  glGetBooleanv(GL_SCISSOR_TEST, &scissor_enabled);
  if (scissor_enabled)
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, false);

  GLboolean color_mask[4] = {GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE};
  glGetBooleanv(GL_COLOR_WRITEMASK, color_mask);
  state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  GLfloat clear_color[4] = {0.0f, 0.0f, 0.0f, 0.0f};
  glGetFloatv(GL_COLOR_CLEAR_VALUE, clear_color);
  glClearColor(1.0f, 0.0f, 1.0f, 1.0f);

  glClear(GL_COLOR_BUFFER_BIT);

  glBindFramebufferEXT(GL_READ_FRAMEBUFFER, validation_fbo_multisample_);
  glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, validation_fbo_);

  BlitFramebufferHelper(
      0, 0, 1, 1, 0, 0, 1, 1, GL_COLOR_BUFFER_BIT, GL_NEAREST);

  glBindFramebufferEXT(GL_FRAMEBUFFER, validation_fbo_);

  unsigned char pixel[3] = {0, 0, 0};
  glReadPixels(0, 0, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);

  glBindFramebufferEXT(GL_FRAMEBUFFER, validation_fbo_multisample_);
  glFramebufferRenderbufferEXT(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_RENDERBUFFER, 0);

  if (scissor_enabled)
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, true);

  state_.SetDeviceColorMask(
      color_mask[0], color_mask[1], color_mask[2], color_mask[3]);
  glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]);
  glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, draw_framebuffer);
  glBindFramebufferEXT(GL_READ_FRAMEBUFFER, read_framebuffer);

  return (pixel[0] == 0xFF &&
      pixel[1] == 0x00 &&
      pixel[2] == 0xFF);
}
