bool GLES2DecoderImpl::ValidateCopyTexFormat(const char* func_name,
                                             GLenum internal_format,
                                             GLenum read_format,
                                             GLenum read_type) {
  std::string output_error_msg;
  if (!ValidateCopyTexFormatHelper(GetFeatureInfo(), internal_format,
                                   read_format, read_type, &output_error_msg)) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, func_name,
                       output_error_msg.c_str());
    return false;
  }
  return true;
}
