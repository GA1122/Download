void GLES2DecoderImpl::DoGetTexParameterfv(
    GLenum target, GLenum pname, GLfloat* params) {
  InitTextureMaxAnisotropyIfNeeded(target, pname);
  glGetTexParameterfv(target, pname, params);
}
