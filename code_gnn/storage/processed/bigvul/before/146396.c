bool WebGLRenderingContextBase::ValidateBlendEquation(const char* function_name,
                                                      GLenum mode) {
  switch (mode) {
    case GL_FUNC_ADD:
    case GL_FUNC_SUBTRACT:
    case GL_FUNC_REVERSE_SUBTRACT:
      return true;
    case GL_MIN_EXT:
    case GL_MAX_EXT:
      if (ExtensionEnabled(kEXTBlendMinMaxName) || IsWebGL2OrHigher())
        return true;
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid mode");
      return false;
    default:
      SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid mode");
      return false;
  }
}
