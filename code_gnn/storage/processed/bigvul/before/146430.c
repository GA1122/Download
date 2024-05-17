bool WebGLRenderingContextBase::ValidateTexImageSourceFormatAndType(
    const char* function_name,
    TexImageFunctionType function_type,
    GLenum internalformat,
    GLenum format,
    GLenum type) {
  if (!is_web_gl2_tex_image_source_formats_types_added_ && IsWebGL2OrHigher()) {
    ADD_VALUES_TO_SET(supported_tex_image_source_internal_formats_,
                      kSupportedInternalFormatsTexImageSourceES3);
    ADD_VALUES_TO_SET(supported_tex_image_source_formats_,
                      kSupportedFormatsTexImageSourceES3);
    ADD_VALUES_TO_SET(supported_tex_image_source_types_,
                      kSupportedTypesTexImageSourceES3);
    is_web_gl2_tex_image_source_formats_types_added_ = true;
  }

  if (!IsWebGL2OrHigher()) {
    AddExtensionSupportedFormatsTypes();
  }

  if (internalformat != 0 &&
      supported_tex_image_source_internal_formats_.find(internalformat) ==
          supported_tex_image_source_internal_formats_.end()) {
    if (function_type == kTexImage) {
      SynthesizeGLError(GL_INVALID_VALUE, function_name,
                        "invalid internalformat");
    } else {
      SynthesizeGLError(GL_INVALID_ENUM, function_name,
                        "invalid internalformat");
    }
    return false;
  }
  if (supported_tex_image_source_formats_.find(format) ==
      supported_tex_image_source_formats_.end()) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid format");
    return false;
  }
  if (supported_tex_image_source_types_.find(type) ==
      supported_tex_image_source_types_.end()) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid type");
    return false;
  }

  return true;
}
