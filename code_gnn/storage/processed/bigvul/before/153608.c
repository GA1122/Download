void GLES2Implementation::DeleteBuffersStub(GLsizei n, const GLuint* buffers) {
  helper_->DeleteBuffersImmediate(n, buffers);
}
