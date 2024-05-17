void FeatureInfo::EnableOESTextureFloatLinear() {
  if (!oes_texture_float_linear_available_)
    return;
  AddExtensionString("GL_OES_texture_float_linear");
  feature_flags_.enable_texture_float_linear = true;
  validators_.texture_sized_texture_filterable_internal_format.AddValue(
      GL_R32F);
  validators_.texture_sized_texture_filterable_internal_format.AddValue(
      GL_RG32F);
  validators_.texture_sized_texture_filterable_internal_format.AddValue(
      GL_RGB32F);
  validators_.texture_sized_texture_filterable_internal_format.AddValue(
      GL_RGBA32F);
}
