bool GLES2DecoderImpl::CheckUniformForApiType(
    const Program::UniformInfo* info,
    const char* function_name,
    Program::UniformApiType api_type) {
  DCHECK(info);
  if ((api_type & info->accepts_api_type) == 0) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "wrong uniform function for type");
    return false;
  }
  return true;
}
