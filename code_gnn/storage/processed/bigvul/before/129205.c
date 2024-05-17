bool BackRenderbuffer::AllocateStorage(const FeatureInfo* feature_info,
                                       const gfx::Size& size,
                                       GLenum format,
                                       GLsizei samples) {
  ScopedGLErrorSuppressor suppressor(
      "BackRenderbuffer::AllocateStorage", state_->GetErrorState());
  ScopedRenderBufferBinder binder(state_, id_);

  uint32 estimated_size = 0;
  if (!renderbuffer_manager_->ComputeEstimatedRenderbufferSize(
           size.width(), size.height(), samples, format, &estimated_size)) {
    return false;
  }

  if (!memory_tracker_.EnsureGPUMemoryAvailable(estimated_size)) {
    return false;
  }

  if (samples <= 1) {
    glRenderbufferStorageEXT(GL_RENDERBUFFER,
                             format,
                             size.width(),
                             size.height());
  } else {
    GLES2DecoderImpl::RenderbufferStorageMultisampleHelper(feature_info,
                                                           GL_RENDERBUFFER,
                                                           samples,
                                                           format,
                                                           size.width(),
                                                           size.height());
  }
  bool success = glGetError() == GL_NO_ERROR;
  if (success) {
    memory_tracker_.TrackMemFree(bytes_allocated_);
    bytes_allocated_ = estimated_size;
    memory_tracker_.TrackMemAlloc(bytes_allocated_);
  }
  return success;
}
