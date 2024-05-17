error::Error GLES2DecoderImpl::HandleTexImage2D(uint32 immediate_data_size,
                                                const void* cmd_data) {
  const gles2::cmds::TexImage2D& c =
      *static_cast<const gles2::cmds::TexImage2D*>(cmd_data);
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::HandleTexImage2D",
      "width", c.width, "height", c.height);
  texture_state_.tex_image_2d_failed = true;
  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLenum internal_format = static_cast<GLenum>(c.internalformat);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLint border = static_cast<GLint>(c.border);
  GLenum format = static_cast<GLenum>(c.format);
  GLenum type = static_cast<GLenum>(c.type);
  uint32 pixels_shm_id = static_cast<uint32>(c.pixels_shm_id);
  uint32 pixels_shm_offset = static_cast<uint32>(c.pixels_shm_offset);
  uint32 pixels_size;
  if (!GLES2Util::ComputeImageDataSizes(
      width, height, 1, format, type, state_.unpack_alignment, &pixels_size,
      NULL, NULL)) {
    return error::kOutOfBounds;
  }
  const void* pixels = NULL;
  if (pixels_shm_id != 0 || pixels_shm_offset != 0) {
    pixels = GetSharedMemoryAs<const void*>(
        pixels_shm_id, pixels_shm_offset, pixels_size);
    if (!pixels) {
      return error::kOutOfBounds;
    }
  }

  TextureManager::DoTextImage2DArguments args = {
    target, level, internal_format, width, height, border, format, type,
    pixels, pixels_size};
  texture_manager()->ValidateAndDoTexImage2D(
      &texture_state_, &state_, &framebuffer_state_, args);

  ExitCommandProcessingEarly();
  return error::kNoError;
}
