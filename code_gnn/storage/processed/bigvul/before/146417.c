bool WebGLRenderingContextBase::ValidateRenderingState(
    const char* function_name) {
  if (!current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, function_name,
                      "no valid shader program in use");
    return false;
  }

  return true;
}
