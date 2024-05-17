bool GLES2DecoderImpl::ValidateCompressedTexDimensions(
    const char* function_name,
    GLint level, GLsizei width, GLsizei height, GLenum format) {
  switch (format) {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: {
      if (!IsValidDXTSize(level, width) || !IsValidDXTSize(level, height)) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name,
            "width or height invalid for level");
        return false;
      }
      return true;
    }
    case GL_ATC_RGB_AMD:
    case GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
    case GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD:
    case GL_ETC1_RGB8_OES: {
      if (width <= 0 || height <= 0) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name,
            "width or height invalid for level");
        return false;
      }
      return true;
    }
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: {
      if (!IsValidPVRTCSize(level, width) ||
          !IsValidPVRTCSize(level, height)) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name,
            "width or height invalid for level");
        return false;
      }
      return true;
    }
    default:
      return false;
  }
}
