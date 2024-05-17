bool WebGLRenderingContextBase::ValidateTexFunc(
    const char* function_name,
    TexImageFunctionType function_type,
    TexFuncValidationSourceType source_type,
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    GLenum format,
    GLenum type,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset) {
  if (!ValidateTexFuncLevel(function_name, target, level))
    return false;

  if (!ValidateTexFuncParameters(function_name, function_type, source_type,
                                 target, level, internalformat, width, height,
                                 depth, border, format, type))
    return false;

  if (function_type == kTexSubImage) {
    if (!ValidateSettableTexFormat(function_name, format))
      return false;
    if (!ValidateSize(function_name, xoffset, yoffset, zoffset))
      return false;
  } else {
    if (source_type != kSourceArrayBufferView) {
      if (!ValidateSettableTexFormat(function_name, format))
        return false;
    }
  }

  return true;
}
