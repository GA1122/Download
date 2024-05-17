void GLES2Implementation::BindVertexArrayOESHelper(GLuint array) {
  bool changed = false;
  if (vertex_array_object_manager_->BindVertexArray(array, &changed)) {
    if (changed) {
      helper_->BindVertexArrayOES(array);
    }
  } else {
    SetGLError(GL_INVALID_OPERATION, "glBindVertexArrayOES",
               "id was not generated with glGenVertexArrayOES");
  }
}
