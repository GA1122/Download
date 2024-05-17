void Compositor::SetDisplayColorSpace(const gfx::ColorSpace& color_space,
                                      float sdr_white_level) {
  gfx::ColorSpace output_color_space = color_space;

#if defined(OS_WIN)
  if (color_space.IsHDR()) {
    bool transparent = SkColorGetA(host_->background_color()) != SK_AlphaOPAQUE;
    output_color_space = transparent ? gfx::ColorSpace::CreateSCRGBLinear()
                                     : gfx::ColorSpace::CreateHDR10();
    output_color_space = output_color_space.GetScaledColorSpace(
        gfx::ColorSpace::kDefaultSDRWhiteLevel / sdr_white_level);
  }
#endif   

  if (output_color_space_ == output_color_space &&
      sdr_white_level_ == sdr_white_level) {
    return;
  }

  output_color_space_ = output_color_space;
  blending_color_space_ = output_color_space_.GetBlendingColorSpace();
  sdr_white_level_ = sdr_white_level;
  host_->SetRasterColorSpace(gfx::ColorSpace::CreateSRGB());
  host_->SetNeedsDisplayOnAllLayers();

  if (context_factory_private_) {
    context_factory_private_->SetDisplayColorSpace(this, blending_color_space_,
                                                   output_color_space_);
  }
}
