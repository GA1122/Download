void WebGLRenderingContextBase::AddExtensionSupportedFormatsTypes() {
  if (!is_oes_texture_float_formats_types_added_ &&
      ExtensionEnabled(kOESTextureFloatName)) {
    ADD_VALUES_TO_SET(supported_types_, kSupportedTypesOESTexFloat);
    ADD_VALUES_TO_SET(supported_tex_image_source_types_,
                      kSupportedTypesOESTexFloat);
    is_oes_texture_float_formats_types_added_ = true;
  }

  if (!is_oes_texture_half_float_formats_types_added_ &&
      ExtensionEnabled(kOESTextureHalfFloatName)) {
    ADD_VALUES_TO_SET(supported_types_, kSupportedTypesOESTexHalfFloat);
    ADD_VALUES_TO_SET(supported_tex_image_source_types_,
                      kSupportedTypesOESTexHalfFloat);
    is_oes_texture_half_float_formats_types_added_ = true;
  }

  if (!is_web_gl_depth_texture_formats_types_added_ &&
      ExtensionEnabled(kWebGLDepthTextureName)) {
    ADD_VALUES_TO_SET(supported_internal_formats_,
                      kSupportedInternalFormatsOESDepthTex);
    ADD_VALUES_TO_SET(supported_tex_image_source_internal_formats_,
                      kSupportedInternalFormatsOESDepthTex);
    ADD_VALUES_TO_SET(supported_formats_, kSupportedFormatsOESDepthTex);
    ADD_VALUES_TO_SET(supported_tex_image_source_formats_,
                      kSupportedFormatsOESDepthTex);
    ADD_VALUES_TO_SET(supported_types_, kSupportedTypesOESDepthTex);
    ADD_VALUES_TO_SET(supported_tex_image_source_types_,
                      kSupportedTypesOESDepthTex);
    is_web_gl_depth_texture_formats_types_added_ = true;
  }

  if (!is_ext_srgb_formats_types_added_ && ExtensionEnabled(kEXTsRGBName)) {
    ADD_VALUES_TO_SET(supported_internal_formats_,
                      kSupportedInternalFormatsEXTsRGB);
    ADD_VALUES_TO_SET(supported_tex_image_source_internal_formats_,
                      kSupportedInternalFormatsEXTsRGB);
    ADD_VALUES_TO_SET(supported_formats_, kSupportedFormatsEXTsRGB);
    ADD_VALUES_TO_SET(supported_tex_image_source_formats_,
                      kSupportedFormatsEXTsRGB);
    is_ext_srgb_formats_types_added_ = true;
  }
}
