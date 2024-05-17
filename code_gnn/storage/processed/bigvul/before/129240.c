void BackTexture::Destroy() {
  if (id_ != 0) {
    ScopedGLErrorSuppressor suppressor("BackTexture::Destroy",
                                       state_->GetErrorState());
    glDeleteTextures(1, &id_);
    id_ = 0;
  }
  memory_tracker_.TrackMemFree(bytes_allocated_);
  bytes_allocated_ = 0;
}
