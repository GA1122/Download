void GLES2DecoderImpl::ProduceFrontBuffer(const Mailbox& mailbox) {
  if (!offscreen_saved_color_texture_.get()) {
    LOG(ERROR) << "Called ProduceFrontBuffer on a non-offscreen context";
    return;
  }
  if (!offscreen_saved_color_texture_info_.get()) {
    GLuint service_id = offscreen_saved_color_texture_->id();
    offscreen_saved_color_texture_info_ = TextureRef::Create(
        texture_manager(), 0, service_id);
    texture_manager()->SetTarget(offscreen_saved_color_texture_info_.get(),
                                 GL_TEXTURE_2D);
    UpdateParentTextureInfo();
  }
  mailbox_manager()->ProduceTexture(
      mailbox, offscreen_saved_color_texture_info_->texture());
}
