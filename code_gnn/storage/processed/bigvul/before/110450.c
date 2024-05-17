void GLES2DecoderImpl::DoActiveTexture(GLenum texture_unit) {
  GLuint texture_index = texture_unit - GL_TEXTURE0;
  if (texture_index >= group_->max_texture_units()) {
    SetGLErrorInvalidEnum(
        "glActiveTexture", texture_unit, "texture_unit");
    return;
  }
  active_texture_unit_ = texture_index;
  glActiveTexture(texture_unit);
}
