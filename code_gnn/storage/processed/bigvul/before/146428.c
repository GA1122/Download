bool WebGLRenderingContextBase::ValidateTexFuncParameters(
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
    GLenum type) {
  if (source_type == kSourceHTMLImageElement ||
      source_type == kSourceHTMLCanvasElement ||
      source_type == kSourceHTMLVideoElement ||
      source_type == kSourceImageData || source_type == kSourceImageBitmap) {
    if (!ValidateTexImageSourceFormatAndType(function_name, function_type,
                                             internalformat, format, type)) {
      return false;
    }
  } else {
    if (!ValidateTexFuncFormatAndType(function_name, function_type,
                                      internalformat, format, type, level)) {
      return false;
    }
  }

  if (!ValidateTexFuncDimensions(function_name, function_type, target, level,
                                 width, height, depth))
    return false;

  if (border) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "border != 0");
    return false;
  }

  return true;
}
