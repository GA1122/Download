void GLES2Implementation::DeleteTexturesStub(GLsizei n,
                                             const GLuint* textures) {
  helper_->DeleteTexturesImmediate(n, textures);
}
