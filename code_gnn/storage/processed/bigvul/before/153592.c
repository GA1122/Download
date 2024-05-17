void GLES2Implementation::CompressedTexSubImage2D(GLenum target,
                                                  GLint level,
                                                  GLint xoffset,
                                                  GLint yoffset,
                                                  GLsizei width,
                                                  GLsizei height,
                                                  GLenum format,
                                                  GLsizei image_size,
                                                  const void* data) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG(
      "[" << GetLogPrefix() << "] glCompressedTexSubImage2D("
          << GLES2Util::GetStringTextureTarget(target) << ", " << level << ", "
          << xoffset << ", " << yoffset << ", " << width << ", " << height
          << ", " << GLES2Util::GetStringCompressedTextureFormat(format) << ", "
          << image_size << ", " << static_cast<const void*>(data) << ")");
  if (width < 0 || height < 0 || level < 0) {
    SetGLError(GL_INVALID_VALUE, "glCompressedTexSubImage2D", "dimension < 0");
    return;
  }
  if (bound_pixel_unpack_transfer_buffer_id_) {
    GLuint offset = ToGLuint(data);
    BufferTracker::Buffer* buffer = GetBoundPixelTransferBufferIfValid(
        bound_pixel_unpack_transfer_buffer_id_, "glCompressedTexSubImage2D",
        offset, image_size);
    if (buffer && buffer->shm_id() != -1) {
      helper_->CompressedTexSubImage2D(
          target, level, xoffset, yoffset, width, height, format, image_size,
          buffer->shm_id(), buffer->shm_offset() + offset);
      buffer->set_last_usage_token(helper_->InsertToken());
      CheckGLError();
    }
    return;
  }
  if (bound_pixel_unpack_buffer_) {
    helper_->CompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                     height, format, image_size, 0,
                                     ToGLuint(data));
  } else if (data) {
    SetBucketContents(kResultBucketId, data, image_size);
    helper_->CompressedTexSubImage2DBucket(target, level, xoffset, yoffset,
                                           width, height, format,
                                           kResultBucketId);
    helper_->SetBucketSize(kResultBucketId, 0);
  } else {
    helper_->CompressedTexSubImage2D(target, level, xoffset, yoffset, width,
                                     height, format, image_size, 0, 0);
  }
  CheckGLError();
}
