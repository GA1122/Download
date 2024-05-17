void GLES2Implementation::DeleteRenderbuffersHelper(
    GLsizei n,
    const GLuint* renderbuffers) {
  if (!GetIdHandler(SharedIdNamespaces::kRenderbuffers)
           ->FreeIds(this, n, renderbuffers,
                     &GLES2Implementation::DeleteRenderbuffersStub)) {
    SetGLError(GL_INVALID_VALUE, "glDeleteRenderbuffers",
               "id not created by this context.");
    return;
  }
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (renderbuffers[ii] == bound_renderbuffer_) {
      bound_renderbuffer_ = 0;
    }
  }
}
