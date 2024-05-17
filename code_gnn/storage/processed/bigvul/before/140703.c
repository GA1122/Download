bool GLES2DecoderImpl::CheckCurrentValuebuffer(const char* function_name) {
  if (!state_.bound_valuebuffer.get()) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, function_name,
                       "no valuebuffer in use");
    return false;
  }
  return true;
}
