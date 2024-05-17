void GLES2Implementation::DeleteBuffersHelper(GLsizei n,
                                              const GLuint* buffers) {
  if (!GetIdHandler(SharedIdNamespaces::kBuffers)
           ->FreeIds(this, n, buffers,
                     &GLES2Implementation::DeleteBuffersStub)) {
    SetGLError(GL_INVALID_VALUE, "glDeleteBuffers",
               "id not created by this context.");
    return;
  }
  for (GLsizei ii = 0; ii < n; ++ii) {
    if (buffers[ii] == bound_array_buffer_) {
      bound_array_buffer_ = 0;
    }
    if (buffers[ii] == bound_atomic_counter_buffer_) {
      bound_atomic_counter_buffer_ = 0;
    }
    if (buffers[ii] == bound_copy_read_buffer_) {
      bound_copy_read_buffer_ = 0;
    }
    if (buffers[ii] == bound_copy_write_buffer_) {
      bound_copy_write_buffer_ = 0;
    }
    if (buffers[ii] == bound_pixel_pack_buffer_) {
      bound_pixel_pack_buffer_ = 0;
    }
    if (buffers[ii] == bound_pixel_unpack_buffer_) {
      bound_pixel_unpack_buffer_ = 0;
    }
    if (buffers[ii] == bound_shader_storage_buffer_) {
      bound_shader_storage_buffer_ = 0;
    }
    if (buffers[ii] == bound_transform_feedback_buffer_) {
      bound_transform_feedback_buffer_ = 0;
    }
    if (buffers[ii] == bound_uniform_buffer_) {
      bound_uniform_buffer_ = 0;
    }
    vertex_array_object_manager_->UnbindBuffer(buffers[ii]);

    BufferTracker::Buffer* buffer = buffer_tracker_->GetBuffer(buffers[ii]);
    if (buffer)
      RemoveTransferBuffer(buffer);

    readback_buffer_shadow_tracker_->RemoveBuffer(buffers[ii]);

    if (buffers[ii] == bound_pixel_unpack_transfer_buffer_id_) {
      bound_pixel_unpack_transfer_buffer_id_ = 0;
    }

    RemoveMappedBufferRangeById(buffers[ii]);
  }
}
