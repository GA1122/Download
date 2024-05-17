error::Error GLES2DecoderImpl::HandleTexSubImage2D(
    uint32_t immediate_data_size, const volatile void* cmd_data) {
  const char* func_name = "glTexSubImage2D";
  const volatile gles2::cmds::TexSubImage2D& c =
      *static_cast<const volatile gles2::cmds::TexSubImage2D*>(cmd_data);
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::HandleTexSubImage2D",
      "width", c.width, "height", c.height);
  GLboolean internal = static_cast<GLboolean>(c.internal);
  if (internal == GL_TRUE && texture_state_.tex_image_failed)
    return error::kNoError;

  GLenum target = static_cast<GLenum>(c.target);
  GLint level = static_cast<GLint>(c.level);
  GLint xoffset = static_cast<GLint>(c.xoffset);
  GLint yoffset = static_cast<GLint>(c.yoffset);
  GLsizei width = static_cast<GLsizei>(c.width);
  GLsizei height = static_cast<GLsizei>(c.height);
  GLenum format = static_cast<GLenum>(c.format);
  GLenum type = static_cast<GLenum>(c.type);
  uint32_t pixels_shm_id = static_cast<uint32_t>(c.pixels_shm_id);
  uint32_t pixels_shm_offset = static_cast<uint32_t>(c.pixels_shm_offset);

  if (width < 0 || height < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, func_name, "dimensions < 0");
    return error::kNoError;
  }

  PixelStoreParams params;
  Buffer* buffer = state_.bound_pixel_unpack_buffer.get();
  if (buffer) {
    if (pixels_shm_id)
      return error::kInvalidArguments;
    if (buffer->GetMappedRange()) {
      LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
          "pixel unpack buffer should not be mapped to client memory");
      return error::kNoError;
    }
    params = state_.GetUnpackParams(ContextState::k2D);
  } else {
    if (!pixels_shm_id && pixels_shm_offset)
      return error::kInvalidArguments;
    params.alignment = state_.unpack_alignment;
  }
  uint32_t pixels_size;
  uint32_t skip_size;
  uint32_t padding;
  if (!GLES2Util::ComputeImageDataSizesES3(width, height, 1,
                                           format, type,
                                           params,
                                           &pixels_size,
                                           nullptr,
                                           nullptr,
                                           &skip_size,
                                           &padding)) {
    return error::kOutOfBounds;
  }
  DCHECK_EQ(0u, skip_size);

  const void* pixels;
  if (pixels_shm_id) {
    pixels = GetSharedMemoryAs<const void*>(
        pixels_shm_id, pixels_shm_offset, pixels_size);
    if (!pixels)
      return error::kOutOfBounds;
  } else {
    DCHECK(buffer || !pixels_shm_offset);
    pixels = reinterpret_cast<const void*>(pixels_shm_offset);
  }

  TextureManager::DoTexSubImageArguments args = {
      target, level, xoffset, yoffset, 0, width, height, 1,
      format, type, pixels, pixels_size, padding,
      TextureManager::DoTexSubImageArguments::kTexSubImage2D};
  texture_manager()->ValidateAndDoTexSubImage(this, &texture_state_, &state_,
                                              &framebuffer_state_,
                                              func_name, args);

  ExitCommandProcessingEarly();
  return error::kNoError;
}