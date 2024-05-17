void BackTexture::Create() {
  DCHECK_EQ(id(), 0u);
  ScopedGLErrorSuppressor suppressor("BackTexture::Create",
                                     decoder_->error_state_.get());
  GLuint id;
  api()->glGenTexturesFn(1, &id);

  GLenum target = Target();
  ScopedTextureBinder binder(&decoder_->state_, decoder_->error_state_.get(),
                             id, target);

  texture_ref_ = TextureRef::Create(decoder_->texture_manager(), 0, id);
  decoder_->texture_manager()->SetTarget(texture_ref_.get(), target);
  decoder_->texture_manager()->SetParameteri(
      "BackTexture::Create", decoder_->error_state_.get(), texture_ref_.get(),
      GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  decoder_->texture_manager()->SetParameteri(
      "BackTexture::Create", decoder_->error_state_.get(), texture_ref_.get(),
      GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  decoder_->texture_manager()->SetParameteri(
      "BackTexture::Create", decoder_->error_state_.get(), texture_ref_.get(),
      GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  decoder_->texture_manager()->SetParameteri(
      "BackTexture::Create", decoder_->error_state_.get(), texture_ref_.get(),
      GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
