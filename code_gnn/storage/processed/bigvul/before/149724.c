void FeatureInfo::EnableCHROMIUMColorBufferFloatRGBA() {
  if (!feature_flags_.chromium_color_buffer_float_rgba)
    return;
  validators_.texture_internal_format.AddValue(GL_RGBA32F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_RGBA32F);
  AddExtensionString("GL_CHROMIUM_color_buffer_float_rgba");
}
