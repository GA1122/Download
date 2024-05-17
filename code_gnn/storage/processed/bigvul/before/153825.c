void GLES2Implementation::TexImage2D(GLenum target,
                                     GLint level,
                                     GLint internalformat,
                                     GLsizei width,
                                     GLsizei height,
                                     GLint border,
                                     GLenum format,
                                     GLenum type,
                                     const void* pixels) {
  const char* func_name = "glTexImage2D";
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG(
      "[" << GetLogPrefix() << "] glTexImage2D("
          << GLES2Util::GetStringTextureTarget(target) << ", " << level << ", "
          << GLES2Util::GetStringTextureInternalFormat(internalformat) << ", "
          << width << ", " << height << ", " << border << ", "
          << GLES2Util::GetStringTextureFormat(format) << ", "
          << GLES2Util::GetStringPixelType(type) << ", "
          << static_cast<const void*>(pixels) << ")");
  if (level < 0 || height < 0 || width < 0) {
    SetGLError(GL_INVALID_VALUE, func_name, "dimension < 0");
    return;
  }
  if (border != 0) {
    SetGLError(GL_INVALID_VALUE, func_name, "border != 0");
    return;
  }
  if ((bound_pixel_unpack_buffer_ || pixels) &&
      (unpack_skip_pixels_ + width >
       (unpack_row_length_ ? unpack_row_length_ : width))) {
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
    SetGLError(GL_INVALID_VALUE, func_name, "image size too large");
    return;
  }

  if (bound_pixel_unpack_buffer_) {
    base::CheckedNumeric<uint32_t> offset = ToGLuint(pixels);
    offset += skip_size;
    if (!offset.IsValid()) {
      SetGLError(GL_INVALID_VALUE, func_name, "skip size too large");
      return;
    }
    helper_->TexImage2D(target, level, internalformat, width, height, format,
                        type, 0, offset.ValueOrDefault(0));
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
      helper_->TexImage2D(target, level, internalformat, width, height, format,
                          type, buffer->shm_id(),
                          buffer->shm_offset() + offset);
      buffer->set_last_usage_token(helper_->InsertToken());
      CheckGLError();
    }
    return;
  }

  if (!pixels || width == 0 || height == 0) {
    helper_->TexImage2D(target, level, internalformat, width, height, format,
                        type, 0, 0);
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

  int32_t shm_id = 0;
  uint32_t shm_offset = 0;
  void* buffer_pointer = nullptr;

  ScopedTransferBufferPtr transfer_alloc(size, helper_, transfer_buffer_);
  ScopedMappedMemoryPtr mapped_alloc(0, helper_, mapped_memory_.get());

  if (transfer_alloc.valid() && transfer_alloc.size() >= size) {
    shm_id = transfer_alloc.shm_id();
    shm_offset = transfer_alloc.offset();
    buffer_pointer = transfer_alloc.address();
  } else if (size < max_extra_transfer_buffer_size_) {
    mapped_alloc.Reset(size);
    if (mapped_alloc.valid()) {
      transfer_alloc.Discard();

      mapped_alloc.SetFlushAfterRelease(true);
      shm_id = mapped_alloc.shm_id();
      shm_offset = mapped_alloc.offset();
      buffer_pointer = mapped_alloc.address();
    }
  }

  if (buffer_pointer) {
    CopyRectToBuffer(pixels, height, unpadded_row_size, padded_row_size,
                     buffer_pointer, service_padded_row_size);
    helper_->TexImage2D(target, level, internalformat, width, height, format,
                        type, shm_id, shm_offset);
    CheckGLError();
    return;
  }

  helper_->TexImage2D(target, level, internalformat, width, height, format,
                      type, 0, 0);
  TexSubImage2DImpl(target, level, 0, 0, width, height, format, type,
                    unpadded_row_size, pixels, padded_row_size, GL_TRUE,
                    &transfer_alloc, service_padded_row_size);
  CheckGLError();
}
