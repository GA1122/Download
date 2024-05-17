error::Error GLES2DecoderImpl::HandleCompressedTexSubImage2D(
    uint32_t immediate_data_size,
    const volatile void* cmd_data) {
  const volatile gles2::cmds::CompressedTexSubImage2D& c =
      *static_cast<const volatile gles2::cmds::CompressedTexSubImage2D*>(
          cmd_data);
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLint xoffset = static_cast<GLint>(c.xoffset);
  GLint yoffset = static_cast<GLint>(c.yoffset);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLenum format = static_cast<GLenum>(c.format);
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
    if (!data_shm_id) {
      return error::kInvalidArguments;
    }
    data = GetSharedMemoryAs<const void*>(
        data_shm_id, data_shm_offset, image_size);
  }
  return DoCompressedTexSubImage(target, level, xoffset, yoffset, 0,
                                 width, height, 1, format, image_size, data,
                                 ContextState::k2D);
}
