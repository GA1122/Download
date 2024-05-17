void GLES2Implementation::CompressedTexSubImage3D(GLenum target,
                                                  GLint level,
                                                  GLint xoffset,
                                                  GLint yoffset,
                                                  GLint zoffset,
                                                  GLsizei width,
                                                  GLsizei height,
                                                  GLsizei depth,
                                                  GLenum format,
                                                  GLsizei image_size,
                                                  const void* data) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG(
      "[" << GetLogPrefix() << "] glCompressedTexSubImage3D("
          << GLES2Util::GetStringTextureTarget(target) << ", " << level << ", "
          << xoffset << ", " << yoffset << ", " << zoffset << ", " << width
          << ", " << height << ", " << depth << ", "
          << GLES2Util::GetStringCompressedTextureFormat(format) << ", "
          << image_size << ", " << static_cast<const void*>(data) << ")");
  if (width < 0 || height < 0 || depth < 0 || level < 0) {
    SetGLError(GL_INVALID_VALUE, "glCompressedTexSubImage3D", "dimension < 0");
    return;
  }
  if (bound_pixel_unpack_transfer_buffer_id_) {
    GLuint offset = ToGLuint(data);
    BufferTracker::Buffer* buffer = GetBoundPixelTransferBufferIfValid(
        bound_pixel_unpack_transfer_buffer_id_, "glCompressedTexSubImage3D",
        offset, image_size);
    if (buffer && buffer->shm_id() != -1) {
      helper_->CompressedTexSubImage3D(
          target, level, xoffset, yoffset, zoffset, width, height, depth,
          format, image_size, buffer->shm_id(), buffer->shm_offset() + offset);
      buffer->set_last_usage_token(helper_->InsertToken());
      CheckGLError();
    }
    return;
  }
  if (bound_pixel_unpack_buffer_) {
    helper_->CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                                     width, height, depth, format, image_size,
                                     0, ToGLuint(data));
  } else if (data) {
    SetBucketContents(kResultBucketId, data, image_size);
    helper_->CompressedTexSubImage3DBucket(target, level, xoffset, yoffset,
                                           zoffset, width, height, depth,
                                           format, kResultBucketId);
    helper_->SetBucketSize(kResultBucketId, 0);
  } else {
    helper_->CompressedTexSubImage3D(target, level, xoffset, yoffset, zoffset,
                                     width, height, depth, format, image_size,
                                     0, 0);
  }
  CheckGLError();
}
