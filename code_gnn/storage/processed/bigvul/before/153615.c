void GLES2Implementation::DeleteSamplersHelper(GLsizei n,
                                               const GLuint* samplers) {
  if (!GetIdHandler(SharedIdNamespaces::kSamplers)
           ->FreeIds(this, n, samplers,
                     &GLES2Implementation::DeleteSamplersStub)) {
    SetGLError(GL_INVALID_VALUE, "glDeleteSamplers",
               "id not created by this context.");
    return;
  }
}
