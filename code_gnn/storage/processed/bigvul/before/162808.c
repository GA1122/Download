void ContextState::RemoveBoundBuffer(Buffer* buffer) {
  DCHECK(buffer);
  vertex_attrib_manager->Unbind(buffer);
  if (bound_array_buffer.get() == buffer) {
    bound_array_buffer = nullptr;
  }
  if (bound_copy_read_buffer.get() == buffer) {
    bound_copy_read_buffer = nullptr;
  }
  if (bound_copy_write_buffer.get() == buffer) {
    bound_copy_write_buffer = nullptr;
  }
  if (bound_pixel_pack_buffer.get() == buffer) {
    bound_pixel_pack_buffer = nullptr;
    UpdatePackParameters();
  }
  if (bound_pixel_unpack_buffer.get() == buffer) {
    bound_pixel_unpack_buffer = nullptr;
    UpdateUnpackParameters();
  }
  if (bound_transform_feedback_buffer.get() == buffer) {
    bound_transform_feedback_buffer = nullptr;
  }
  if (bound_transform_feedback.get()) {
    bound_transform_feedback->RemoveBoundBuffer(buffer);
  }
  if (bound_uniform_buffer.get() == buffer) {
    bound_uniform_buffer = nullptr;
  }
}
