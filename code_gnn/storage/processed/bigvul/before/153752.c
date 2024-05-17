bool GLES2Implementation::IsBufferReservedId(GLuint id) {
  return vertex_array_object_manager_->IsReservedId(id);
}
