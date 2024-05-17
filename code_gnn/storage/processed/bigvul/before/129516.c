bool GLES2DecoderImpl::ValidateTexSubImage2D(
    error::Error* error,
    const char* function_name,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    const void * data) {
  (*error) = error::kNoError;
  if (!validators_->texture_target.IsValid(target)) {
    LOCAL_SET_GL_ERROR_INVALID_ENUM(function_name, target, "target");
    return false;
  }
  if (width < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "width < 0");
    return false;
  }
  if (height < 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "height < 0");
    return false;
  }
  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        function_name, "unknown texture for target");
    return false;
  }
  Texture* texture = texture_ref->texture();
  GLenum current_type = 0;
  GLenum internal_format = 0;
  if (!texture->GetLevelType(target, level, &current_type, &internal_format)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "level does not exist.");
    return false;
  }
  if (!texture_manager()->ValidateTextureParameters(state_.GetErrorState(),
      function_name, format, type, internal_format, level)) {
    return false;
  }
  if (type != current_type) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        function_name, "type does not match type of texture.");
    return false;
  }
  if (async_pixel_transfer_manager_->AsyncTransferIsInProgress(texture_ref)) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        function_name, "async upload pending for texture");
    return false;
  }
  if (!texture->ValidForTexture(
          target, level, xoffset, yoffset, width, height, type)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_VALUE, function_name, "bad dimensions.");
    return false;
  }
  if ((GLES2Util::GetChannelsForFormat(format) &
       (GLES2Util::kDepth | GLES2Util::kStencil)) != 0) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        function_name, "can not supply data for depth or stencil textures");
    return false;
  }
  if (data == NULL) {
    (*error) = error::kOutOfBounds;
    return false;
  }
  return true;
}
