void GLES2DecoderImpl::RestoreStateForAttrib(
    GLuint attrib_index, bool restore_array_binding) {
  const VertexAttrib* attrib =
      state_.vertex_attrib_manager->GetVertexAttrib(attrib_index);
  if (restore_array_binding) {
    const void* ptr = reinterpret_cast<const void*>(attrib->offset());
    Buffer* buffer = attrib->buffer();
    glBindBuffer(GL_ARRAY_BUFFER, buffer ? buffer->service_id() : 0);
    glVertexAttribPointer(
        attrib_index, attrib->size(), attrib->type(), attrib->normalized(),
        attrib->gl_stride(), ptr);
  }
  if (attrib->divisor())
    glVertexAttribDivisorANGLE(attrib_index, attrib->divisor());
  glBindBuffer(
      GL_ARRAY_BUFFER, state_.bound_array_buffer.get() ?
          state_.bound_array_buffer->service_id() : 0);

  if (attrib_index != 0 ||
      gfx::GetGLImplementation() == gfx::kGLImplementationEGLGLES2) {
    if (attrib->enabled()) {
      glEnableVertexAttribArray(attrib_index);
    } else {
      glDisableVertexAttribArray(attrib_index);
    }
  }
}
