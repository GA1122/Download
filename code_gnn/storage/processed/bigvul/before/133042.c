bool RenderWidgetHostViewAura::SwapBuffersPrepare(
    const gfx::Rect& surface_rect,
    float surface_scale_factor,
    const gfx::Rect& damage_rect,
    const std::string& mailbox_name,
    const BufferPresentedCallback& ack_callback) {
  if (last_swapped_surface_size_ != surface_rect.size()) {
    DLOG_IF(ERROR, damage_rect != surface_rect) << "Expected full damage rect";
    skipped_damage_.setEmpty();
    last_swapped_surface_size_ = surface_rect.size();
    last_swapped_surface_scale_factor_ = surface_scale_factor;
  }

  if (ShouldSkipFrame(ConvertSizeToDIP(surface_scale_factor,
                                       surface_rect.size())) ||
      mailbox_name.empty()) {
    skipped_damage_.op(RectToSkIRect(damage_rect), SkRegion::kUnion_Op);
    ack_callback.Run(true, scoped_refptr<ui::Texture>());
    return false;
  }

  ImageTransportFactory* factory = ImageTransportFactory::GetInstance();
  current_surface_ =
      factory->CreateTransportClient(surface_scale_factor);
  if (!current_surface_.get()) {
    LOG(ERROR) << "Failed to create ImageTransport texture";
    ack_callback.Run(true, scoped_refptr<ui::Texture>());
    return false;
  }

  current_surface_->Consume(mailbox_name, surface_rect.size());
  released_front_lock_ = NULL;
  UpdateExternalTexture();

  return true;
}
