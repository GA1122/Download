void GLES2Implementation::DrawElementsImpl(GLenum mode,
                                           GLsizei count,
                                           GLenum type,
                                           const void* indices,
                                           const char* func_name) {
  if (count < 0) {
    SetGLError(GL_INVALID_VALUE, func_name, "count < 0");
    return;
  }
  bool simulated = false;
  GLuint offset = ToGLuint(indices);
  if (count > 0) {
    if (vertex_array_object_manager_->bound_element_array_buffer() != 0 &&
        !ValidateOffset(func_name, reinterpret_cast<GLintptr>(indices))) {
      return;
    }
    if (!vertex_array_object_manager_->SetupSimulatedIndexAndClientSideBuffers(
            func_name, this, helper_, count, type, 0, indices, &offset,
            &simulated)) {
      return;
    }
  }
  helper_->DrawElements(mode, count, type, offset);
  RestoreElementAndArrayBuffers(simulated);
  CheckGLError();
}
