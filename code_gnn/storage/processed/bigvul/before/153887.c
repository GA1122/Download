bool GLES2DecoderImpl::CanUseCopyTextureCHROMIUMInternalFormat(
    GLenum dest_internal_format) {
  switch (dest_internal_format) {
    case GL_RGB:
    case GL_RGBA:
    case GL_RGB8:
    case GL_RGBA8:
    case GL_BGRA_EXT:
    case GL_BGRA8_EXT:
    case GL_SRGB_EXT:
    case GL_SRGB_ALPHA_EXT:
    case GL_R8:
    case GL_R8UI:
    case GL_RG8:
    case GL_RG8UI:
    case GL_SRGB8:
    case GL_RGB565:
    case GL_RGB8UI:
    case GL_SRGB8_ALPHA8:
    case GL_RGB5_A1:
    case GL_RGBA4:
    case GL_RGBA8UI:
    case GL_RGB9_E5:
    case GL_R16F:
    case GL_R32F:
    case GL_RG16F:
    case GL_RG32F:
    case GL_RGB16F:
    case GL_RGB32F:
    case GL_RGBA16F:
    case GL_RGBA32F:
    case GL_R11F_G11F_B10F:
      return true;
    default:
      return false;
  }
}
