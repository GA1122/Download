void GLES2Implementation::DeleteRenderbuffersStub(GLsizei n,
                                                  const GLuint* renderbuffers) {
  helper_->DeleteRenderbuffersImmediate(n, renderbuffers);
}
