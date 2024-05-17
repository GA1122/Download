error::Error GLES2DecoderImpl::HandleCompressedTexImage3D(
    uint32_t immediate_data_size, const volatile void* cmd_data) {
  if (!feature_info_->IsWebGL2OrES3Context())
    return error::kUnknownCommand;
  const volatile gles2::cmds::CompressedTexImage3D& c =
      *static_cast<const volatile gles2::cmds::CompressedTexImage3D*>(cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLenum internal_format = static_cast<GLenum>(c.internalformat);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLsizei depth = static_cast<GLsizei>(c.depth);
  GLint border = static_cast<GLint>(c.border);
  GLsizei image_size = static_cast<GLsizei>(c.imageSize);
  uint32_t data_shm_id = c.data_shm_id;
  uint32_t data_shm_offset = c.data_shm_offset;

  const void* data;
  if (state_.bound_pixel_unpack_buffer.get()) {
    if (data_shm_id) {
      return error::kInvalidArguments;
    }
    data = reinterpret_cast<const void*>(data_shm_offset);
  } else {
    if (!data_shm_id && data_shm_offset) {
      return error::kInvalidArguments;
    }
    data = GetSharedMemoryAs<const void*>(
        data_shm_id, data_shm_offset, image_size);
  }
  return DoCompressedTexImage(target, level, internal_format, width, height,
                              depth, border, image_size, data,
                              ContextState::k3D);
}