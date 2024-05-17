void BackTexture::Create() {
  ScopedGLErrorSuppressor suppressor("BackTexture::Create",
                                     state_->GetErrorState());
  Destroy();
  glGenTextures(1, &id_);
  ScopedTextureBinder binder(state_, id_, GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexImage2D(
      GL_TEXTURE_2D, 0, GL_RGBA, 16, 16, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  bytes_allocated_ = 16u * 16u * 4u;
  memory_tracker_.TrackMemAlloc(bytes_allocated_);
}
