void GLES2DecoderImpl::DoFramebufferTextureLayer(
    GLenum target, GLenum attachment, GLuint client_texture_id,
    GLint level, GLint layer) {
  GLuint service_id = 0;
  TextureRef* texture_ref = NULL;
  if (client_texture_id) {
    texture_ref = GetTexture(client_texture_id);
    if (!texture_ref) {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION,
          "glFramebufferTextureLayer", "unknown texture_ref");
      return;
    }
    service_id = texture_ref->service_id();
  }
  glFramebufferTextureLayer(target, attachment, service_id, level, layer);
}
