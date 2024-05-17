void GLES2Implementation::ReadPixels(GLint xoffset,
                                     GLint yoffset,
                                     GLsizei width,
                                     GLsizei height,
                                     GLenum format,
                                     GLenum type,
                                     void* pixels) {
  const char* func_name = "glReadPixels";
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glReadPixels(" << xoffset << ", "
                     << yoffset << ", " << width << ", " << height << ", "
                     << GLES2Util::GetStringReadPixelFormat(format) << ", "
                     << GLES2Util::GetStringPixelType(type) << ", "
                     << static_cast<const void*>(pixels) << ")");
  if (width < 0 || height < 0) {
    SetGLError(GL_INVALID_VALUE, func_name, "dimensions < 0");
    return;
  }

  if (pack_skip_pixels_ + width >
      (pack_row_length_ ? pack_row_length_ : width)) {
    SetGLError(GL_INVALID_OPERATION, func_name,
               "invalid pack params combination");
    return;
  }


  TRACE_EVENT0("gpu", "GLES2::ReadPixels");
  typedef cmds::ReadPixels::Result Result;

  uint32_t size;
  uint32_t unpadded_row_size;
  uint32_t padded_row_size;
  uint32_t skip_size;
  PixelStoreParams params;
  params.alignment = pack_alignment_;
  params.row_length = pack_row_length_;
  params.skip_pixels = pack_skip_pixels_;
  params.skip_rows = pack_skip_rows_;
  if (!GLES2Util::ComputeImageDataSizesES3(
          width, height, 1, format, type, params, &size, &unpadded_row_size,
          &padded_row_size, &skip_size, nullptr)) {
    SetGLError(GL_INVALID_VALUE, func_name, "size too large.");
    return;
  }

  if (bound_pixel_pack_buffer_) {
    base::CheckedNumeric<GLuint> offset = ToGLuint(pixels);
    offset += skip_size;
    if (!offset.IsValid()) {
      SetGLError(GL_INVALID_VALUE, func_name, "skip size too large.");
      return;
    }
    helper_->ReadPixels(xoffset, yoffset, width, height, format, type, 0,
                        offset.ValueOrDefault(0), 0, 0, false);
    InvalidateReadbackBufferShadowDataCHROMIUM(bound_pixel_pack_buffer_);

    CheckGLError();
    return;
  }

  uint32_t service_padded_row_size = 0;
  if (pack_row_length_ > 0 && pack_row_length_ != width) {
    if (!GLES2Util::ComputeImagePaddedRowSize(
            width, format, type, pack_alignment_, &service_padded_row_size)) {
      SetGLError(GL_INVALID_VALUE, func_name, "size too large.");
      return;
    }
  } else {
    service_padded_row_size = padded_row_size;
  }

  if (bound_pixel_pack_transfer_buffer_id_) {
    if (pack_row_length_ > 0 || pack_skip_pixels_ > 0 || pack_skip_rows_ > 0) {
      SetGLError(GL_INVALID_OPERATION, func_name,
                 "No ES3 pack parameters with pixel pack transfer buffer.");
      return;
    }
    DCHECK_EQ(0u, skip_size);
    GLuint offset = ToGLuint(pixels);
    BufferTracker::Buffer* buffer = GetBoundPixelTransferBufferIfValid(
        bound_pixel_pack_transfer_buffer_id_, func_name, offset, size);
    if (buffer && buffer->shm_id() != -1) {
      helper_->ReadPixels(xoffset, yoffset, width, height, format, type,
                          buffer->shm_id(), buffer->shm_offset() + offset, 0, 0,
                          true);
      CheckGLError();
    }
    return;
  }

  if (!pixels) {
    SetGLError(GL_INVALID_OPERATION, func_name, "pixels = NULL");
    return;
  }

  int8_t* dest = reinterpret_cast<int8_t*>(pixels);
  dest += skip_size;

  GLsizei remaining_rows = height;
  GLint y_index = yoffset;
  uint32_t group_size = GLES2Util::ComputeImageGroupSize(format, type);
  uint32_t skip_row_bytes = 0;
  if (xoffset < 0) {
    skip_row_bytes = static_cast<uint32_t>(-xoffset) * group_size;
  }
  do {
    GLsizei desired_size =
        remaining_rows == 0 ? 0
                            : service_padded_row_size * (remaining_rows - 1) +
                                  unpadded_row_size;
    ScopedTransferBufferPtr buffer(desired_size, helper_, transfer_buffer_);
    if (!buffer.valid()) {
      break;
    }
    GLint num_rows = ComputeNumRowsThatFitInBuffer(
        service_padded_row_size, unpadded_row_size, buffer.size(),
        remaining_rows);
    auto result = GetResultAs<Result>();
    if (!result) {
      break;
    }
    result->success = 0;   
    result->row_length = 0;
    result->num_rows = 0;
    helper_->ReadPixels(xoffset, y_index, width, num_rows, format, type,
                        buffer.shm_id(), buffer.offset(), GetResultShmId(),
                        result.offset(), false);
    WaitForCmd();
    if (!result->success) {
      break;
    }
    if (remaining_rows == 0) {
      break;
    }
    const uint8_t* src = static_cast<const uint8_t*>(buffer.address());
    if (padded_row_size == unpadded_row_size &&
        (pack_row_length_ == 0 || pack_row_length_ == width) &&
        result->row_length == width && result->num_rows == num_rows) {
      uint32_t copy_size = unpadded_row_size * num_rows;
      memcpy(dest, src, copy_size);
      dest += copy_size;
    } else if (result->row_length > 0 && result->num_rows > 0) {
      uint32_t copy_row_size = result->row_length * group_size;
      uint32_t copy_last_row_size = copy_row_size;
      if (copy_row_size + skip_row_bytes > padded_row_size) {
        copy_row_size = padded_row_size - skip_row_bytes;
      }
      GLint copied_rows = 0;
      for (GLint yy = 0; yy < num_rows; ++yy) {
        if (y_index + yy >= 0 && copied_rows < result->num_rows) {
          if (yy + 1 == num_rows && remaining_rows == num_rows) {
            memcpy(dest + skip_row_bytes, src + skip_row_bytes,
                   copy_last_row_size);
          } else {
            memcpy(dest + skip_row_bytes, src + skip_row_bytes, copy_row_size);
          }
          ++copied_rows;
        }
        dest += padded_row_size;
        src += service_padded_row_size;
      }
      DCHECK_EQ(result->num_rows, copied_rows);
    }
    y_index += num_rows;
    remaining_rows -= num_rows;
  } while (remaining_rows);
  CheckGLError();
}
