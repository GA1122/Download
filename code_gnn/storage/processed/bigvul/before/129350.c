void GLES2DecoderImpl::FinishReadPixels(
    const cmds::ReadPixels& c,
    GLuint buffer) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::FinishReadPixels");
  GLsizei width = c.width;
  GLsizei height = c.height;
  GLenum format = c.format;
  GLenum type = c.type;
  typedef cmds::ReadPixels::Result Result;
  uint32 pixels_size;
  Result* result = NULL;
  if (c.result_shm_id != 0) {
    result = GetSharedMemoryAs<Result*>(
        c.result_shm_id, c.result_shm_offset, sizeof(*result));
    if (!result) {
      if (buffer != 0) {
        glDeleteBuffersARB(1, &buffer);
      }
      return;
    }
  }
  GLES2Util::ComputeImageDataSizes(
      width, height, format, type, state_.pack_alignment, &pixels_size,
      NULL, NULL);
  void* pixels = GetSharedMemoryAs<void*>(
      c.pixels_shm_id, c.pixels_shm_offset, pixels_size);
  if (!pixels) {
    if (buffer != 0) {
      glDeleteBuffersARB(1, &buffer);
    }
    return;
  }

  if (buffer != 0) {
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, buffer);
    void* data;
    if (features().map_buffer_range) {
      data = glMapBufferRange(
          GL_PIXEL_PACK_BUFFER_ARB, 0, pixels_size, GL_MAP_READ_BIT);
    } else {
      data = glMapBuffer(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY);
    }
    memcpy(pixels, data, pixels_size);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER_ARB);
    glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);
    glDeleteBuffersARB(1, &buffer);
  }

  if (result != NULL) {
    *result = true;
  }

  GLenum read_format = GetBoundReadFrameBufferInternalFormat();
  uint32 channels_exist = GLES2Util::GetChannelsForFormat(read_format);
  if ((channels_exist & 0x0008) == 0 &&
      workarounds().clear_alpha_in_readpixels) {
    uint32 temp_size;

    uint32 unpadded_row_size;
    uint32 padded_row_size;
    if (!GLES2Util::ComputeImageDataSizes(
            width, 2, format, type, state_.pack_alignment, &temp_size,
            &unpadded_row_size, &padded_row_size)) {
      return;
    }

    uint32 channel_count = 0;
    uint32 alpha_channel = 0;
    switch (format) {
      case GL_RGBA:
      case GL_BGRA_EXT:
        channel_count = 4;
        alpha_channel = 3;
        break;
      case GL_ALPHA:
        channel_count = 1;
        alpha_channel = 0;
        break;
    }

    if (channel_count > 0) {
      switch (type) {
        case GL_UNSIGNED_BYTE:
          WriteAlphaData<uint8>(
              pixels, height, channel_count, alpha_channel, unpadded_row_size,
              padded_row_size, 0xFF);
          break;
        case GL_FLOAT:
          WriteAlphaData<float>(
              pixels, height, channel_count, alpha_channel, unpadded_row_size,
              padded_row_size, 1.0f);
          break;
        case GL_HALF_FLOAT:
          WriteAlphaData<uint16>(
              pixels, height, channel_count, alpha_channel, unpadded_row_size,
              padded_row_size, 0x3C00);
          break;
      }
    }
  }
}
