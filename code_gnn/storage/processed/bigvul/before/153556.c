void GLES2Implementation::ActiveTexture(GLenum texture) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glActiveTexture("
                     << GLES2Util::GetStringEnum(texture) << ")");
  GLuint texture_index = texture - GL_TEXTURE0;
  if (texture_index >=
      static_cast<GLuint>(capabilities_.max_combined_texture_image_units)) {
    SetGLErrorInvalidEnum("glActiveTexture", texture, "texture");
    return;
  }

  active_texture_unit_ = texture_index;
  helper_->ActiveTexture(texture);
  CheckGLError();
}
