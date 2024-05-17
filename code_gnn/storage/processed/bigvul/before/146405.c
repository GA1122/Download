bool WebGLRenderingContextBase::ValidateDrawArrays(const char* function_name) {
  if (isContextLost())
    return false;

  if (!ValidateStencilSettings(function_name))
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
