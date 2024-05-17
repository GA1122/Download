GLenum BackTexture::Target() {
  return decoder_->should_use_native_gmb_for_backbuffer_
             ? decoder_->GetContextGroup()
                   ->image_factory()
                   ->RequiredTextureType()
             : GL_TEXTURE_2D;
}
