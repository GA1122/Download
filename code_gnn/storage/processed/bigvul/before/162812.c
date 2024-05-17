void ContextState::RestoreBufferBindings() const {
  if (vertex_attrib_manager.get()) {
    Buffer* element_array_buffer =
        vertex_attrib_manager->element_array_buffer();
    api()->glBindBufferFn(GL_ELEMENT_ARRAY_BUFFER,
                          GetBufferId(element_array_buffer));
  }
  api()->glBindBufferFn(GL_ARRAY_BUFFER, GetBufferId(bound_array_buffer.get()));
  if (feature_info_->IsES3Capable()) {
    api()->glBindBufferFn(GL_COPY_READ_BUFFER,
                          GetBufferId(bound_copy_read_buffer.get()));
    api()->glBindBufferFn(GL_COPY_WRITE_BUFFER,
                          GetBufferId(bound_copy_write_buffer.get()));
    api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER,
                          GetBufferId(bound_pixel_pack_buffer.get()));
    UpdatePackParameters();
    api()->glBindBufferFn(GL_PIXEL_UNPACK_BUFFER,
                          GetBufferId(bound_pixel_unpack_buffer.get()));
    UpdateUnpackParameters();
    api()->glBindBufferFn(GL_TRANSFORM_FEEDBACK_BUFFER,
                          GetBufferId(bound_transform_feedback_buffer.get()));
    api()->glBindBufferFn(GL_UNIFORM_BUFFER,
                          GetBufferId(bound_uniform_buffer.get()));
  }
}
