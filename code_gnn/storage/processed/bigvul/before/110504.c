void GLES2DecoderImpl::DoTexParameterf(
    GLenum target, GLenum pname, GLfloat param) {
  TextureManager::TextureInfo* info = GetTextureInfoForTarget(target);
  if (!info) {
    SetGLError(GL_INVALID_VALUE, "glTexParameterf", "unknown texture");
    return;
  }

  if (!texture_manager()->SetParameter(
      info, pname, static_cast<GLint>(param))) {
    SetGLErrorInvalidEnum("glTexParameterf", pname, "pname");
    return;
  }
  glTexParameterf(target, pname, param);
}
