WebLayer* DrawingBuffer::PlatformLayer() {
  if (!layer_) {
    layer_ =
        Platform::Current()->CompositorSupport()->CreateExternalTextureLayer(
            this);

    layer_->SetOpaque(!want_alpha_channel_);
    layer_->SetBlendBackgroundColor(want_alpha_channel_);
    layer_->SetPremultipliedAlpha(premultiplied_alpha_);
    layer_->SetNearestNeighbor(filter_quality_ == kNone_SkFilterQuality);
    GraphicsLayer::RegisterContentsLayer(layer_->Layer());
  }

  return layer_->Layer();
}
