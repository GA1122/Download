void GLES2DecoderImpl::DeleteBuffersHelper(
    GLsizei n, const GLuint* client_ids) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    BufferManager::BufferInfo* buffer = GetBufferInfo(client_ids[ii]);
    if (buffer && !buffer->IsDeleted()) {
      vertex_attrib_manager_->Unbind(buffer);
      if (bound_array_buffer_ == buffer) {
        bound_array_buffer_ = NULL;
      }
      if (bound_element_array_buffer_ == buffer) {
        bound_element_array_buffer_ = NULL;
      }
      RemoveBufferInfo(client_ids[ii]);
    }
  }
}
