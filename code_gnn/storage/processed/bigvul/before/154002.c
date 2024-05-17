void GLES2DecoderImpl::DoFramebufferTextureLayer(
    GLenum target, GLenum attachment, GLuint client_texture_id,
    GLint level, GLint layer) {
  const char* function_name = "glFramebufferTextureLayer";

  TextureRef* texture_ref = nullptr;
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(target);
  if (!framebuffer) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "no framebuffer bound.");
    return;
  }
  GLuint service_id = 0;
  GLenum texture_target = 0;
  if (client_texture_id) {
    texture_ref = GetTexture(client_texture_id);
    if (!texture_ref) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, function_name, "unknown texture");
      return;
    }
    service_id = texture_ref->service_id();

    texture_target = texture_ref->texture()->target();
    switch (texture_target) {
      case GL_TEXTURE_3D:
      case GL_TEXTURE_2D_ARRAY:
        break;
      default:
        LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
            "texture is neither TEXTURE_3D nor TEXTURE_2D_ARRAY");
        return;
    }
    if (!texture_manager()->ValidForTarget(texture_target, level,
                                           0, 0, layer)) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_VALUE, function_name, "invalid level or layer");
      return;
    }
  }
  api()->glFramebufferTextureLayerFn(target, attachment, service_id, level,
                                     layer);
  if (attachment == GL_DEPTH_STENCIL_ATTACHMENT) {
    framebuffer->AttachTextureLayer(
        GL_DEPTH_ATTACHMENT, texture_ref, texture_target, level, layer);
    framebuffer->AttachTextureLayer(
        GL_STENCIL_ATTACHMENT, texture_ref, texture_target, level, layer);
  } else {
    framebuffer->AttachTextureLayer(
        attachment, texture_ref, texture_target, level, layer);
  }
  if (framebuffer == framebuffer_state_.bound_draw_framebuffer.get()) {
    framebuffer_state_.clear_state_dirty = true;
  }
}
