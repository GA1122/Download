bool GLES2DecoderImpl::ValidateCompressedTexFuncData(
    const char* function_name,
    GLsizei width, GLsizei height, GLenum format, size_t size) {
  unsigned int bytes_required = 0;

  switch (format) {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: {
        int num_blocks_across =
            (width + kS3TCBlockWidth - 1) / kS3TCBlockWidth;
        int num_blocks_down =
            (height + kS3TCBlockHeight - 1) / kS3TCBlockHeight;
        int num_blocks = num_blocks_across * num_blocks_down;
        bytes_required = num_blocks * kS3TCDXT1BlockSize;
        break;
      }
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
    default:
      SetGLErrorInvalidEnum(function_name, format, "format");
      return false;
  }

  if (size != bytes_required) {
    SetGLError(
        GL_INVALID_VALUE, function_name, "size is not correct for dimensions");
    return false;
  }

  return true;
}
