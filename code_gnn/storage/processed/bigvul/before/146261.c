GLenum WebGLRenderingContextBase::ConvertTexInternalFormat(
    GLenum internalformat,
    GLenum type) {
  if (type == GL_FLOAT && internalformat == GL_RGBA &&
      ExtensionsUtil()->IsExtensionEnabled(
          "GL_CHROMIUM_color_buffer_float_rgba"))
    return GL_RGBA32F_EXT;
  if (type == GL_FLOAT && internalformat == GL_RGB &&
      ExtensionsUtil()->IsExtensionEnabled(
          "GL_CHROMIUM_color_buffer_float_rgb"))
    return GL_RGB32F_EXT;
  return internalformat;
}
