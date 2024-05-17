void GLES2DecoderImpl::DoDisableVertexAttribArray(GLuint index) {
  if (state_.vertex_attrib_manager->Enable(index, false)) {
    if (index != 0 ||
        gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2) {
      glDisableVertexAttribArray(index);
    }
  } else {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE,
        "glDisableVertexAttribArray", "index out of range");
  }
}
