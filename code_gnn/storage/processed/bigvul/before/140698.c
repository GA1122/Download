bool BackTexture::AllocateStorage(
    const gfx::Size& size, GLenum format, bool zero) {
  DCHECK_NE(id_, 0u);
  ScopedGLErrorSuppressor suppressor("BackTexture::AllocateStorage",
                                     state_->GetErrorState());
  ScopedTextureBinder binder(state_, id_, GL_TEXTURE_2D);
  uint32 image_size = 0;
  GLES2Util::ComputeImageDataSizes(
      size.width(), size.height(), 1, format, GL_UNSIGNED_BYTE, 8, &image_size,
      NULL, NULL);

  if (!memory_tracker_.EnsureGPUMemoryAvailable(image_size)) {
    return false;
  }

  scoped_ptr<char[]> zero_data;
  if (zero) {
    zero_data.reset(new char[image_size]);
    memset(zero_data.get(), 0, image_size);
  }

  glTexImage2D(GL_TEXTURE_2D,
               0,   
               format,
               size.width(),
               size.height(),
               0,   
               format,
               GL_UNSIGNED_BYTE,
               zero_data.get());

  size_ = size;

  bool success = glGetError() == GL_NO_ERROR;
  if (success) {
    memory_tracker_.TrackMemFree(bytes_allocated_);
    bytes_allocated_ = image_size;
    memory_tracker_.TrackMemAlloc(bytes_allocated_);
  }
  return success;
}
