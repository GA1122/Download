void GLES2DecoderImpl::DoFramebufferTexture2DCommon(
    const char* name, GLenum target, GLenum attachment, GLenum textarget,
    GLuint client_texture_id, GLint level, GLsizei samples) {
  if (samples > renderbuffer_manager()->max_samples()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glFramebufferTexture2DMultisample", "samples too large");
    return;
  }
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (!framebuffer) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        name, "no framebuffer bound.");
    return;
  }
  GLuint service_id = 0;
  TextureRef* texture_ref = NULL;
  if (client_texture_id) {
    texture_ref = GetTexture(client_texture_id);
    if (!texture_ref) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          name, "unknown texture_ref");
      return;
    }
    service_id = texture_ref->service_id();
  }

  if (!texture_manager()->ValidForTarget(textarget, level, 0, 0, 1)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        name, "level out of range");
    return;
  }

  if (texture_ref)
    DoWillUseTexImageIfNeeded(texture_ref->texture(), textarget);

  LOCAL_COPY_REAL_GL_ERRORS_TO_WRAPPER(name);
  if (0 == samples) {
    glFramebufferTexture2DEXT(target, attachment, textarget, service_id, level);
  } else {
    if (features().use_img_for_multisampled_render_to_texture) {
      glFramebufferTexture2DMultisampleIMG(target, attachment, textarget,
          service_id, level, samples);
    } else {
      glFramebufferTexture2DMultisampleEXT(target, attachment, textarget,
          service_id, level, samples);
    }
  }
  GLenum error = LOCAL_PEEK_GL_ERROR(name);
  if (error == GL_NO_ERROR) {
    framebuffer->AttachTexture(attachment, texture_ref, textarget, level,
         samples);
  }
  if (framebuffer == framebuffer_state_.bound_draw_framebuffer.get()) {
    framebuffer_state_.clear_state_dirty = true;
  }

  if (texture_ref)
    DoDidUseTexImageIfNeeded(texture_ref->texture(), textarget);

  OnFboChanged();
}
