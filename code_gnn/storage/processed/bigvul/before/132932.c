void RenderWidgetHostViewAura::DidRecreateLayer(ui::Layer *old_layer,
                                                ui::Layer *new_layer) {
  float mailbox_scale_factor;
  cc::TextureMailbox old_mailbox =
      old_layer->GetTextureMailbox(&mailbox_scale_factor);
  scoped_refptr<ui::Texture> old_texture = old_layer->external_texture();
  if (old_texture.get()) {
    ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
    GLHelper* gl_helper = factory->GetGLHelper();
    scoped_refptr<ui::Texture> new_texture;
    if (host_->is_accelerated_compositing_active() &&
        gl_helper && current_surface_.get()) {
      GLuint texture_id =
          gl_helper->CopyTexture(current_surface_->PrepareTexture(),
                                 current_surface_->size());
      if (texture_id) {
        new_texture = factory->CreateOwnedTexture(
          current_surface_->size(),
          current_surface_->device_scale_factor(), texture_id);
      }
    }
    if (new_texture.get())
      old_layer->SetExternalTexture(new_texture.get());
    else
      old_layer->SetShowPaintedContent();
    new_layer->SetExternalTexture(old_texture.get());
  } else if (old_mailbox.IsSharedMemory()) {
    base::SharedMemory* old_buffer = old_mailbox.shared_memory();
    const size_t size = old_mailbox.shared_memory_size_in_bytes();

    scoped_ptr<base::SharedMemory> new_buffer(new base::SharedMemory);
    new_buffer->CreateAndMapAnonymous(size);

    if (old_buffer->memory() && new_buffer->memory()) {
      memcpy(new_buffer->memory(), old_buffer->memory(), size);
      base::SharedMemory* new_buffer_raw_ptr = new_buffer.get();
      scoped_ptr<cc::SingleReleaseCallback> callback =
          cc::SingleReleaseCallback::Create(base::Bind(MailboxReleaseCallback,
                                                       Passed(&new_buffer)));
      cc::TextureMailbox new_mailbox(new_buffer_raw_ptr,
                                     old_mailbox.shared_memory_size());
      new_layer->SetTextureMailbox(new_mailbox,
                                   callback.Pass(),
                                   mailbox_scale_factor);
    }
  } else if (frame_provider_.get()) {
    new_layer->SetShowDelegatedContent(frame_provider_.get(),
                                       current_frame_size_);
  }
}
