bool WebGLRenderingContextBase::ValidateShaderSource(const String& string) {
  for (size_t i = 0; i < string.length(); ++i) {
    if (IsWebGL2OrHigher() && string[i] == '\\') {
      continue;
    }
    if (!ValidateCharacter(string[i])) {
      SynthesizeGLError(GL_INVALID_VALUE, "shaderSource", "string not ASCII");
      return false;
    }
  }
  return true;
}
