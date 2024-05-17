bool CreateImageValidInternalFormat(GLenum internalformat,
                                    const Capabilities& capabilities) {
  switch (internalformat) {
    case GL_R16_EXT:
      return capabilities.texture_norm16;
    case GL_RGB10_A2_EXT:
      return capabilities.image_xr30 || capabilities.image_xb30;
    case GL_RED:
    case GL_RG_EXT:
    case GL_RGB:
    case GL_RGBA:
    case GL_RGB_YCBCR_422_CHROMIUM:
    case GL_RGB_YCBCR_420V_CHROMIUM:
    case GL_RGB_YCRCB_420_CHROMIUM:
    case GL_BGRA_EXT:
      return true;
    default:
      return false;
  }
}
