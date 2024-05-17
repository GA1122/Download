error::Error GLES2DecoderPassthroughImpl::DoBindFramebuffer(
    GLenum target,
    GLuint framebuffer) {
  CheckErrorCallbackState();
  api()->glBindFramebufferEXTFn(
      target, GetFramebufferServiceID(api(), framebuffer, &framebuffer_id_map_,
                                      bind_generates_resource_));
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  bool draw_framebuffer_changed = false;
  switch (target) {
    case GL_FRAMEBUFFER_EXT:
      draw_framebuffer_changed = true;
      bound_draw_framebuffer_ = framebuffer;
      bound_read_framebuffer_ = framebuffer;
      break;

    case GL_DRAW_FRAMEBUFFER:
      draw_framebuffer_changed = true;
      bound_draw_framebuffer_ = framebuffer;
      break;

    case GL_READ_FRAMEBUFFER:
      bound_read_framebuffer_ = framebuffer;
      break;

    default:
      NOTREACHED();
      break;
  }

  if (draw_framebuffer_changed && bound_draw_framebuffer_ != framebuffer &&
      (bound_draw_framebuffer_ == 0 || framebuffer == 0)) {
    ApplySurfaceDrawOffset();
  }

  return error::kNoError;
}
