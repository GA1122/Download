bool GLES2DecoderImpl::ValidateAsyncTransfer(
    const char* function_name,
    TextureRef* texture_ref,
    GLenum target,
    GLint level,
    const void * data) {
  if (GL_TEXTURE_2D != target) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, target, "target");
    return false;
  }
  if (level != 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "level != 0");
    return false;
  }
  if (data == NULL) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name, "buffer == 0");
    return false;
  }
  if (!texture_ref ||
      async_pixel_transfer_manager_->AsyncTransferIsInProgress(texture_ref)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        function_name, "transfer already in progress");
    return false;
  }
  return true;
}
