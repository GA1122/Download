void GLES2Implementation::BindBufferHelper(GLenum target, GLuint buffer_id) {
  bool changed = false;
  switch (target) {
    case GL_ARRAY_BUFFER:
      if (bound_array_buffer_ != buffer_id) {
        bound_array_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_ATOMIC_COUNTER_BUFFER:
      if (bound_atomic_counter_buffer_ != buffer_id) {
        bound_atomic_counter_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_COPY_READ_BUFFER:
      if (bound_copy_read_buffer_ != buffer_id) {
        bound_copy_read_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_COPY_WRITE_BUFFER:
      if (bound_copy_write_buffer_ != buffer_id) {
        bound_copy_write_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_ELEMENT_ARRAY_BUFFER:
      changed = vertex_array_object_manager_->BindElementArray(buffer_id);
      break;
    case GL_PIXEL_PACK_BUFFER:
      if (bound_pixel_pack_buffer_ != buffer_id) {
        bound_pixel_pack_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_PIXEL_PACK_TRANSFER_BUFFER_CHROMIUM:
      bound_pixel_pack_transfer_buffer_id_ = buffer_id;
      break;
    case GL_PIXEL_UNPACK_BUFFER:
      if (bound_pixel_unpack_buffer_ != buffer_id) {
        bound_pixel_unpack_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_PIXEL_UNPACK_TRANSFER_BUFFER_CHROMIUM:
      bound_pixel_unpack_transfer_buffer_id_ = buffer_id;
      break;
    case GL_SHADER_STORAGE_BUFFER:
      if (bound_shader_storage_buffer_ != buffer_id) {
        bound_shader_storage_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
      if (bound_transform_feedback_buffer_ != buffer_id) {
        bound_transform_feedback_buffer_ = buffer_id;
        changed = true;
      }
      break;
    case GL_UNIFORM_BUFFER:
      if (bound_uniform_buffer_ != buffer_id) {
        bound_uniform_buffer_ = buffer_id;
        changed = true;
      }
      break;
    default:
      changed = true;
      break;
  }
  if (changed) {
    GetIdHandler(SharedIdNamespaces::kBuffers)
        ->MarkAsUsedForBind(this, target, buffer_id,
                            &GLES2Implementation::BindBufferStub);
  }
}
