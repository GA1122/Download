void GLES2DecoderImpl::DoTexParameterfv(
    GLenum target, GLenum pname, const GLfloat* params) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameterfv", "unknown texture");
    return;
  }

  if (!texture_manager()->SetParameter(
      info, pname, static_cast<GLint>(params[0]))) {
    SetGLErrorInvalidEnum("glTexParameterfv", pname, "pname");
    return;
  }
  glTexParameterfv(target, pname, params);
}
