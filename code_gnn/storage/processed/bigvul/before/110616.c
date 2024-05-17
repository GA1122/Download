GLenum GLES2DecoderImpl::PeekGLError() {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    SetGLError(error, "", "");
  }
  return error;
}
