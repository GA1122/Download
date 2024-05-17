bool GLES2DecoderImpl::ValidateCompressedTexFuncData(
    const char* function_name,
    GLsizei width, GLsizei height, GLenum format, size_t size) {
  unsigned int bytes_required = 0;

  switch (format) {
    case GL_ATC_RGB_AMD:
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_ETC1_RGB8_OES: {
        int num_blocks_across =
            (width + kS3TCBlockWidth - 1) / kS3TCBlockWidth;
        int num_blocks_down =
            (height + kS3TCBlockHeight - 1) / kS3TCBlockHeight;
        int num_blocks = num_blocks_across * num_blocks_down;
        bytes_required = num_blocks * kS3TCDXT1BlockSize;
        break;
      }
    case GL_ATC_RGBA_EXPLICIT_ALPHA_AMD:
    case GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: {
        int num_blocks_across =
            (width + kS3TCBlockWidth - 1) / kS3TCBlockWidth;
        int num_blocks_down =
            (height + kS3TCBlockHeight - 1) / kS3TCBlockHeight;
        int num_blocks = num_blocks_across * num_blocks_down;
        bytes_required = num_blocks * kS3TCDXT3AndDXT5BlockSize;
        break;
      }
    case GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG: {
        bytes_required = (std::max(width, 8) * std::max(height, 8) * 4 + 7)/8;
        break;
      }
    case GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
    case GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG: {
        bytes_required = (std::max(width, 16) * std::max(height, 8) * 2 + 7)/8;
        break;
      }
    default:
      LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, format, "format");
      return false;
  }

  if (size != bytes_required) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_VALUE, function_name, "size is not correct for dimensions");
    return false;
  }

  return true;
}
