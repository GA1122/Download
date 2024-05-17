bool WebGLRenderingContextBase::ValidateTexFuncLevel(const char* function_name,
                                                     GLenum target,
                                                     GLint level) {
  if (level < 0) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "level < 0");
    return false;
  }
  GLint max_level = GetMaxTextureLevelForTarget(target);
  if (max_level && level >= max_level) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "level out of range");
    return false;
  }
  return true;
}
