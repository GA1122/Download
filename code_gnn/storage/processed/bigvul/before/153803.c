void GLES2Implementation::RestoreElementAndArrayBuffers(bool restore) {
  if (restore) {
    RestoreArrayBuffer(restore);
    if (vertex_array_object_manager_->bound_element_array_buffer() == 0) {
      helper_->BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
  }
}
