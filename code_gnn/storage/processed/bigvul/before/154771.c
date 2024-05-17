error::Error GLES2DecoderPassthroughImpl::DoReadPixelsAsync(
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    GLsizei bufsize,
    GLsizei* length,
    GLsizei* columns,
    GLsizei* rows,
    uint32_t pixels_shm_id,
    uint32_t pixels_shm_offset,
    uint32_t result_shm_id,
    uint32_t result_shm_offset) {
  DCHECK(feature_info_->feature_flags().use_async_readpixels &&
         bound_buffers_[GL_PIXEL_PACK_BUFFER] == 0);

  CheckErrorCallbackState();
  ScopedPackStateRowLengthReset reset_row_length(
      api(), bufsize != 0 && feature_info_->gl_version_info().is_es3);

  PendingReadPixels pending_read_pixels;
  pending_read_pixels.pixels_shm_id = pixels_shm_id;
  pending_read_pixels.pixels_shm_offset = pixels_shm_offset;
  pending_read_pixels.result_shm_id = result_shm_id;
  pending_read_pixels.result_shm_offset = result_shm_offset;

  api()->glGenBuffersARBFn(1, &pending_read_pixels.buffer_service_id);
  api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB,
                        pending_read_pixels.buffer_service_id);

  const GLenum usage_hint = feature_info_->gl_version_info().IsAtLeastGLES(3, 0)
                                ? GL_STREAM_READ
                                : GL_STATIC_DRAW;

  const uint32_t bytes_per_pixel =
      GLES2Util::ComputeImageGroupSize(format, type);
  if (bytes_per_pixel == 0) {
    InsertError(GL_INVALID_ENUM, "Invalid ReadPixels format or type.");
    return error::kNoError;
  }

  if (width < 0 || height < 0) {
    InsertError(GL_INVALID_VALUE, "Width and height cannot be negative.");
    return error::kNoError;
  }

  if (!base::CheckMul(bytes_per_pixel, width, height)
           .AssignIfValid(&pending_read_pixels.pixels_size)) {
    return error::kOutOfBounds;
  }

  api()->glBufferDataFn(GL_PIXEL_PACK_BUFFER_ARB,
                        pending_read_pixels.pixels_size, nullptr, usage_hint);

  api()->glReadPixelsFn(x, y, width, height, format, type, nullptr);

  api()->glBindBufferFn(GL_PIXEL_PACK_BUFFER_ARB, 0);

  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  pending_read_pixels.fence = gl::GLFence::Create();

  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  pending_read_pixels_.push_back(std::move(pending_read_pixels));
  return error::kNoError;
}
