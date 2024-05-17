void FeatureInfo::EnableCHROMIUMColorBufferFloatRGB() {
  if (!feature_flags_.chromium_color_buffer_float_rgb)
    return;
  validators_.texture_internal_format.AddValue(GL_RGB32F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_RGB32F);
  AddExtensionString("GL_CHROMIUM_color_buffer_float_rgb");
}
