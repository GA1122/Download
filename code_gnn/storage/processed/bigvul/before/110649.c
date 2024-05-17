bool GLES2DecoderImpl::ValidateTextureParameters(
    const char* function_name,
    GLenum target, GLenum format, GLenum type, GLint level) {
  if (!feature_info_->GetTextureFormatValidator(format).IsValid(type)) {
      SetGLError(GL_INVALID_OPERATION, function_name,
                 (std::string("invalid type ") +
                  GLES2Util::GetStringEnum(type) + " for format " +
                  GLES2Util::GetStringEnum(format)).c_str());
      return false;
  }

  uint32 channels = GLES2Util::GetChannelsForFormat(format);
  if ((channels & (GLES2Util::kDepth | GLES2Util::kStencil)) != 0 && level) {
    SetGLError(GL_INVALID_OPERATION, function_name,
               (std::string("invalid type ") +
                GLES2Util::GetStringEnum(type) + " for format " +
                GLES2Util::GetStringEnum(format)).c_str());
    return false;
  }
  return true;
}
