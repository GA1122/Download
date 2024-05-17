bool WebGLRenderingContextBase::ValidateSize(const char* function_name,
                                             GLint x,
                                             GLint y,
                                             GLint z) {
  if (x < 0 || y < 0 || z < 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "size < 0");
    return false;
  }
  return true;
}
