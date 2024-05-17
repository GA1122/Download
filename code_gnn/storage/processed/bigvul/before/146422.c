bool WebGLRenderingContextBase::ValidateString(const char* function_name,
                                               const String& string) {
  for (size_t i = 0; i < string.length(); ++i) {
    if (!ValidateCharacter(string[i])) {
      SynthesizeGLError(GL_INVALID_VALUE, function_name, "string not ASCII");
      return false;
    }
  }
  return true;
}
