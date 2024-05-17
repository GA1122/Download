bool GLES2DecoderImpl::ValidateCompressedTexSubDimensions(
    const char* function_name,
    GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset,
    GLsizei width, GLsizei height, GLsizei depth, GLenum format,
    Texture* texture) {
  const char* error_message = "";
  if (!::gpu::gles2::ValidateCompressedTexSubDimensions(
          target, level, xoffset, yoffset, zoffset, width, height, depth,
          format, texture, feature_info_->IsWebGLContext(), &error_message)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name, error_message);
    return false;
  }
  return true;
}
