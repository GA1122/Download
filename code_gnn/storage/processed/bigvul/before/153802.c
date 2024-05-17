void GLES2Implementation::RestoreArrayBuffer(bool restore) {
  if (restore) {
    helper_->BindBuffer(GL_ARRAY_BUFFER, bound_array_buffer_);
  }
}
