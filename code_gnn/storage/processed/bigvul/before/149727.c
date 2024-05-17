void FeatureInfo::EnableEXTColorBufferFloat() {
  if (!ext_color_buffer_float_available_)
    return;
  AddExtensionString("GL_EXT_color_buffer_float");
  validators_.render_buffer_format.AddValue(GL_R16F);
  validators_.render_buffer_format.AddValue(GL_RG16F);
  validators_.render_buffer_format.AddValue(GL_RGBA16F);
  validators_.render_buffer_format.AddValue(GL_R32F);
  validators_.render_buffer_format.AddValue(GL_RG32F);
  validators_.render_buffer_format.AddValue(GL_RGBA32F);
  validators_.render_buffer_format.AddValue(GL_R11F_G11F_B10F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(GL_R16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(GL_RG16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_RGBA16F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(GL_R32F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(GL_RG32F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_RGBA32F);
  validators_.texture_sized_color_renderable_internal_format.AddValue(
      GL_R11F_G11F_B10F);
  feature_flags_.enable_color_buffer_float = true;
}
