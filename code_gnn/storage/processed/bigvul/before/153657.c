void GLES2Implementation::GenVertexArraysOESHelper(GLsizei n,
                                                   const GLuint* arrays) {
  vertex_array_object_manager_->GenVertexArrays(n, arrays);
}
