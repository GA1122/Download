bool GLES2DecoderImpl::ValidateCompressedTexDimensions(
    const char* function_name,
    GLint level, GLsizei width, GLsizei height, GLenum format) {
  switch (format) {
    case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
    case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: {
      if (!IsValidDXTSize(level, width) || !IsValidDXTSize(level, height)) {
        SetGLError(
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
