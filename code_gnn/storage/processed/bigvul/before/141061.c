StyleResolver& Document::EnsureStyleResolver() const {
  return style_engine_->EnsureResolver();
}
