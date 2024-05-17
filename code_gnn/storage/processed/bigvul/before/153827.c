void GLES2Implementation::TexSubImage2D(GLenum target,
                                        GLint level,
                                        GLint xoffset,
                                        GLint yoffset,
                                        GLsizei width,
                                        GLsizei height,
                                        GLenum format,
                                        GLenum type,
                                        const void* pixels) {
  const char* func_name = "glTexSubImage2D";
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glTexSubImage2D("
                     << GLES2Util::GetStringTextureTarget(target) << ", "
                     << level << ", " << xoffset << ", " << yoffset << ", "
                     << width << ", " << height << ", "
                     << GLES2Util::GetStringTextureFormat(format) << ", "
                     << GLES2Util::GetStringPixelType(type) << ", "
                     << static_cast<const void*>(pixels) << ")");

  if (level < 0 || height < 0 || width < 0 || xoffset < 0 || yoffset < 0) {
    SetGLError(GL_INVALID_VALUE, func_name, "dimension < 0");
    return;
  }
  if (unpack_skip_pixels_ + width >
      (unpack_row_length_ ? unpack_row_length_ : width)) {
    SetGLError(GL_INVALID_OPERATION, func_name,
               "invalid unpack params combination");
    return;
  }

  uint32_t size;
  uint32_t unpadded_row_size;
  uint32_t padded_row_size;
  uint32_t skip_size;
  PixelStoreParams params = GetUnpackParameters(k2D);
  if (!GLES2Util::ComputeImageDataSizesES3(
          width, height, 1, format, type, params, &size, &unpadded_row_size,
          &padded_row_size, &skip_size, nullptr)) {
    SetGLError(GL_INVALID_VALUE, func_name, "image size to large");
    return;
  }

  if (bound_pixel_unpack_buffer_) {
    base::CheckedNumeric<uint32_t> offset = ToGLuint(pixels);
    offset += skip_size;
    if (!offset.IsValid()) {
      SetGLError(GL_INVALID_VALUE, func_name, "skip size too large");
      return;
    }
    helper_->TexSubImage2D(target, level, xoffset, yoffset, width, height,
                           format, type, 0, offset.ValueOrDefault(0), false);
    CheckGLError();
    return;
  }

  if (bound_pixel_unpack_transfer_buffer_id_) {
    if (unpack_row_length_ > 0 || unpack_image_height_ > 0 ||
        unpack_skip_pixels_ > 0 || unpack_skip_rows_ > 0 ||
        unpack_skip_images_ > 0) {
      SetGLError(GL_INVALID_OPERATION, func_name,
                 "No ES3 pack parameters with pixel unpack transfer buffer.");
      return;
    }
    DCHECK_EQ(0u, skip_size);
    GLuint offset = ToGLuint(pixels);
    BufferTracker::Buffer* buffer = GetBoundPixelTransferBufferIfValid(
        bound_pixel_unpack_transfer_buffer_id_, func_name, offset, size);
    if (buffer && buffer->shm_id() != -1) {
      helper_->TexSubImage2D(target, level, xoffset, yoffset, width, height,
                             format, type, buffer->shm_id(),
                             buffer->shm_offset() + offset, false);
      buffer->set_last_usage_token(helper_->InsertToken());
      CheckGLError();
    }
    return;
  }

  if (width == 0 || height == 0) {
    helper_->TexSubImage2D(target, level, xoffset, yoffset, width, height,
                           format, type, 0, 0, false);
    CheckGLError();
    return;
  }

  uint32_t service_padded_row_size;
  if (unpack_row_length_ > 0 && unpack_row_length_ != width) {
    PixelStoreParams service_params;
    service_params.alignment = unpack_alignment_;
    if (!GLES2Util::ComputeImageDataSizesES3(
            width, height, 1, format, type, service_params, &size, nullptr,
            &service_padded_row_size, nullptr, nullptr)) {
      SetGLError(GL_INVALID_VALUE, func_name, "image size too large");
      return;
    }
  } else {
    service_padded_row_size = padded_row_size;
  }

  pixels = reinterpret_cast<const int8_t*>(pixels) + skip_size;

  ScopedTransferBufferPtr buffer(size, helper_, transfer_buffer_);
  base::CheckedNumeric<GLint> checked_xoffset = xoffset;
  checked_xoffset += width;
  if (!checked_xoffset.IsValid()) {
    SetGLError(GL_INVALID_VALUE, "TexSubImage2D", "xoffset + width overflows");
    return;
  }
  base::CheckedNumeric<GLint> checked_yoffset = yoffset;
  checked_yoffset += height;
  if (!checked_yoffset.IsValid()) {
    SetGLError(GL_INVALID_VALUE, "TexSubImage2D", "yoffset + height overflows");
    return;
  }
  TexSubImage2DImpl(target, level, xoffset, yoffset, width, height, format,
                    type, unpadded_row_size, pixels, padded_row_size, GL_FALSE,
                    &buffer, service_padded_row_size);
  CheckGLError();
}
