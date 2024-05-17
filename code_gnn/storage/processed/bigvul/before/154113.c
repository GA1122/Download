void GLES2DecoderImpl::EmulateVertexArrayState() {
  for (uint32_t vv = 0; vv < group_->max_vertex_attribs(); ++vv) {
    RestoreStateForAttrib(vv, true);
  }

  Buffer* element_array_buffer =
      state_.vertex_attrib_manager->element_array_buffer();
  api()->glBindBufferFn(
      GL_ELEMENT_ARRAY_BUFFER,
      element_array_buffer ? element_array_buffer->service_id() : 0);
}
