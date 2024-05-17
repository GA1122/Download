void GLES2DecoderImpl::DoEnableVertexAttribArray(GLuint index) {
  if (state_.vertex_attrib_manager->Enable(index, true)) {
    glEnableVertexAttribArray(index);
  } else {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, "glEnableVertexAttribArray", "index out of range");
  }
}
