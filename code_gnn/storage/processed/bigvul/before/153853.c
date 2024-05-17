void GLES2Implementation::Viewport(GLint x,
                                   GLint y,
                                   GLsizei width,
                                   GLsizei height) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glViewport(" << x << ", " << y
                     << ", " << width << ", " << height << ")");
  if (width < 0 || height < 0) {
    SetGLError(GL_INVALID_VALUE, "glViewport", "negative width/height");
    return;
  }
  state_.SetViewport(x, y, width, height);
  helper_->Viewport(x, y, width, height);
  CheckGLError();
}
