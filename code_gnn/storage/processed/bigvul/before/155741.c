void Compositor::SetDisplayColorSpace(const gfx::ColorSpace& color_space) {
  if (output_color_space_ == color_space)
    return;
  output_color_space_ = color_space;
  blending_color_space_ = output_color_space_.GetBlendingColorSpace();
  host_->SetRasterColorSpace(gfx::ColorSpace::CreateSRGB());
  host_->SetNeedsDisplayOnAllLayers();

  if (context_factory_private_) {
    context_factory_private_->SetDisplayColorSpace(this, blending_color_space_,
                                                   output_color_space_);
  }
}
