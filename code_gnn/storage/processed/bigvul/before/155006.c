bool WebGLRenderingContextBase::ValidateValueFitNonNegInt32(
    const char* function_name,
    const char* param_name,
    int64_t value) {
  if (value < 0) {
    String error_msg = String(param_name) + " < 0";
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      error_msg.Ascii().data());
    return false;
  }
  if (value > static_cast<int64_t>(std::numeric_limits<int>::max())) {
    String error_msg = String(param_name) + " more than 32-bit";
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      error_msg.Ascii().data());
    return false;
  }
  return true;
}
