void GLES2DecoderImpl::RestoreStateForAttrib(
    GLuint attrib_index, bool restore_array_binding) {
  const VertexAttrib* attrib =
      state_.vertex_attrib_manager->GetVertexAttrib(attrib_index);
  if (restore_array_binding) {
    const void* ptr = reinterpret_cast<const void*>(attrib->offset());
    Buffer* buffer = attrib->buffer();
    api()->glBindBufferFn(GL_ARRAY_BUFFER, buffer ? buffer->service_id() : 0);
    api()->glVertexAttribPointerFn(attrib_index, attrib->size(), attrib->type(),
                                   attrib->normalized(), attrib->gl_stride(),
                                   ptr);
  }

  DCHECK(attrib->divisor() == 0 ||
      feature_info_->feature_flags().angle_instanced_arrays);

  if (feature_info_->feature_flags().angle_instanced_arrays)
    api()->glVertexAttribDivisorANGLEFn(attrib_index, attrib->divisor());
  api()->glBindBufferFn(GL_ARRAY_BUFFER,
                        state_.bound_array_buffer.get()
                            ? state_.bound_array_buffer->service_id()
                            : 0);

  if (attrib_index != 0 || gl_version_info().BehavesLikeGLES()) {
    if (attrib->enabled_in_driver()) {
      api()->glEnableVertexAttribArrayFn(attrib_index);
    } else {
      api()->glDisableVertexAttribArrayFn(attrib_index);
    }
  }
}
