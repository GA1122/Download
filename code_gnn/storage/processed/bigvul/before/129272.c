void GLES2DecoderImpl::DoDrawBuffersEXT(
    GLsizei count, const GLenum* bufs) {
  if (count > static_cast<GLsizei>(group_->max_draw_buffers())) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glDrawBuffersEXT", "greater than GL_MAX_DRAW_BUFFERS_EXT");
    return;
  }

  Framebuffer* framebuffer = GetFramebufferInfoForTarget(GL_FRAMEBUFFER);
  if (framebuffer) {
    for (GLsizei i = 0; i < count; ++i) {
      if (bufs[i] != static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + i) &&
          bufs[i] != GL_NONE) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION,
            "glDrawBuffersEXT",
            "bufs[i] not GL_NONE or GL_COLOR_ATTACHMENTi_EXT");
        return;
      }
    }
    glDrawBuffersARB(count, bufs);
    framebuffer->SetDrawBuffers(count, bufs);
  } else {   
    if (count > 1 ||
        (bufs[0] != GL_BACK && bufs[0] != GL_NONE)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          "glDrawBuffersEXT",
          "more than one buffer or bufs not GL_NONE or GL_BACK");
      return;
    }
    GLenum mapped_buf = bufs[0];
    if (GetBackbufferServiceId() != 0 &&   
        bufs[0] == GL_BACK) {
      mapped_buf = GL_COLOR_ATTACHMENT0;
    }
    glDrawBuffersARB(count, &mapped_buf);
    group_->set_draw_buffer(bufs[0]);
  }
}
