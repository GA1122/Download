bool GLES2DecoderImpl::ValidateCompressedTexSubDimensions(
    const char* function_name,
    GLenum target, GLint level, GLint xoffset, GLint yoffset,
    GLsizei width, GLsizei height, GLenum format,
    Texture* texture) {
  if (xoffset < 0 || yoffset < 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, function_name, "xoffset or yoffset < 0");
    return false;
  }

  switch (format) {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: {
      const int kBlockWidth = 4;
      const int kBlockHeight = 4;
      if ((xoffset % kBlockWidth) || (yoffset % kBlockHeight)) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name,
            "xoffset or yoffset not multiple of 4");
        return false;
      }
      GLsizei tex_width = 0;
      GLsizei tex_height = 0;
      if (!texture->GetLevelSize(target, level, &tex_width, &tex_height) ||
          width - xoffset > tex_width ||
          height - yoffset > tex_height) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name, "dimensions out of range");
        return false;
      }
      return ValidateCompressedTexDimensions(
          function_name, level, width, height, format);
    }
    case GL_ATC_RGB_AMD:
    case GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
    case GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD: {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, function_name,
          "not supported for ATC textures");
      return false;
    }
    case GL_ETC1_RGB8_OES: {
      LOCAL_SET_GL_ERROR(
          GL_INVALID_OPERATION, function_name,
          "not supported for ECT1_RGB8_OES textures");
      return false;
    }
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: {
      if ((xoffset != 0) || (yoffset != 0)) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name,
            "xoffset and yoffset must be zero");
        return false;
      }
      GLsizei tex_width = 0;
      GLsizei tex_height = 0;
      if (!texture->GetLevelSize(target, level, &tex_width, &tex_height) ||
          width != tex_width ||
          height != tex_height) {
        LOCAL_SET_GL_ERROR(
            GL_INVALID_OPERATION, function_name,
            "dimensions must match existing texture level dimensions");
        return false;
      }
      return ValidateCompressedTexDimensions(
          function_name, level, width, height, format);
    }
    default:
      return false;
  }
}
