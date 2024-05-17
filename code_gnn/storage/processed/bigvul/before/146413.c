bool WebGLRenderingContextBase::ValidateLocationLength(
    const char* function_name,
    const String& string) {
  const unsigned max_web_gl_location_length = GetMaxWebGLLocationLength();
  if (string.length() > max_web_gl_location_length) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "location length > 256");
    return false;
  }
  return true;
}
