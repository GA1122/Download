void FeatureInfo::EnableEXTColorBufferHalfFloat() {
  if (!ext_color_buffer_half_float_available_)
    return;
  AddExtensionString("GL_EXT_color_buffer_half_float");
  validators_.render_buffer_format.AddValue(GL_R16F);
  validators_.render_buffer_format.AddValue(GL_RG16F);
  validators_.render_buffer_format.AddValue(GL_RGB16F);
  validators_.render_buffer_format.AddValue(GL_RGBA16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(GL_R16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(GL_RG16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_RGB16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_RGBA16F);
  feature_flags_.enable_color_buffer_half_float = true;
}
