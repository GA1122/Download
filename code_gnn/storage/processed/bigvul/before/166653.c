bool WebGLRenderingContextBase::ValidateDrawElements(const char* function_name,
                                                     GLenum type,
                                                     long long offset) {
  if (isContextLost())
    return false;

  if (type == GL_UNSIGNED_INT && !IsWebGL2OrHigher() &&
      !ExtensionEnabled(kOESElementIndexUintName)) {
    SynthesizeGLError(GL_INVALID_ENUM, function_name, "invalid type");
    return false;
  }

  if (!ValidateValueFitNonNegInt32(function_name, "offset", offset))
    return false;

  if (!ValidateRenderingState(function_name)) {
    return false;
  }

  const char* reason = "framebuffer incomplete";
  if (framebuffer_binding_ && framebuffer_binding_->CheckDepthStencilStatus(
                                  &reason) != GL_FRAMEBUFFER_COMPLETE) {
    SynthesizeGLError(GL_INVALID_FRAMEBUFFER_OPERATION, function_name, reason);
    return false;
  }

  return true;
}
