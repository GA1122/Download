bool GLES2DecoderImpl::ClearLevelUsingGL(Texture* texture,
                                         uint32_t channels,
                                         unsigned target,
                                         int level,
                                         int xoffset,
                                         int yoffset,
                                         int width,
                                         int height) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::ClearLevelUsingGL");
  GLenum fb_target = GetDrawFramebufferTarget();
  GLuint fb = 0;
  api()->glGenFramebuffersEXTFn(1, &fb);
  api()->glBindFramebufferEXTFn(fb_target, fb);

  bool have_color = (channels & GLES2Util::kRGBA) != 0;
  if (have_color) {
    api()->glFramebufferTexture2DEXTFn(fb_target, GL_COLOR_ATTACHMENT0, target,
                                       texture->service_id(), level);
  }
  bool have_depth = (channels & GLES2Util::kDepth) != 0;
  if (have_depth) {
    api()->glFramebufferTexture2DEXTFn(fb_target, GL_DEPTH_ATTACHMENT, target,
                                       texture->service_id(), level);
  }
  bool have_stencil = (channels & GLES2Util::kStencil) != 0;
  if (have_stencil) {
    api()->glFramebufferTexture2DEXTFn(fb_target, GL_STENCIL_ATTACHMENT, target,
                                       texture->service_id(), level);
  }
  bool result = false;
  if (api()->glCheckFramebufferStatusEXTFn(fb_target) ==
      GL_FRAMEBUFFER_COMPLETE) {
    state_.SetDeviceColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    api()->glClearColorFn(0.0, 0.0, 0.0, 0.0);
    api()->glClearStencilFn(0);
    state_.SetDeviceStencilMaskSeparate(GL_FRONT, kDefaultStencilMask);
    state_.SetDeviceStencilMaskSeparate(GL_BACK, kDefaultStencilMask);
    api()->glClearDepthFn(1.0f);
    state_.SetDeviceDepthMask(GL_TRUE);
    state_.SetDeviceCapabilityState(GL_SCISSOR_TEST, true);
    gfx::Vector2d scissor_offset = GetBoundFramebufferDrawOffset();
    api()->glScissorFn(xoffset + scissor_offset.x(),
                       yoffset + scissor_offset.y(), width, height);
    ClearDeviceWindowRectangles();

    api()->glClearFn((have_color ? GL_COLOR_BUFFER_BIT : 0) |
                     (have_depth ? GL_DEPTH_BUFFER_BIT : 0) |
                     (have_stencil ? GL_STENCIL_BUFFER_BIT : 0));
    result = true;
  }
  RestoreClearState();
  api()->glDeleteFramebuffersEXTFn(1, &fb);
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(fb_target);
  GLuint fb_service_id =
      framebuffer ? framebuffer->service_id() : GetBackbufferServiceId();
  api()->glBindFramebufferEXTFn(fb_target, fb_service_id);
  return result;
}
