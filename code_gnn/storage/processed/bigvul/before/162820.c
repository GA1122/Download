void ContextState::RestoreVertexAttribArrays(
    const scoped_refptr<VertexAttribManager> attrib_manager) const {
  GLuint vao_service_id = attrib_manager->service_id();
  DCHECK(vao_service_id == 0);

  if (feature_info_->feature_flags().native_vertex_array_object)
    api()->glBindVertexArrayOESFn(vao_service_id);

  for (size_t attrib_index = 0; attrib_index < attrib_manager->num_attribs();
       ++attrib_index) {
    const VertexAttrib* attrib = attrib_manager->GetVertexAttrib(attrib_index);

    Buffer* buffer = attrib->buffer();
    GLuint buffer_service_id = buffer ? buffer->service_id() : 0;
    api()->glBindBufferFn(GL_ARRAY_BUFFER, buffer_service_id);
    const void* ptr = reinterpret_cast<const void*>(attrib->offset());
    api()->glVertexAttribPointerFn(attrib_index, attrib->size(), attrib->type(),
                                   attrib->normalized(), attrib->gl_stride(),
                                   ptr);

    if (feature_info_->feature_flags().angle_instanced_arrays)
      api()->glVertexAttribDivisorANGLEFn(attrib_index, attrib->divisor());

    if (attrib->enabled_in_driver()) {
      api()->glEnableVertexAttribArrayFn(attrib_index);
    } else {
      api()->glDisableVertexAttribArrayFn(attrib_index);
    }
  }
}
