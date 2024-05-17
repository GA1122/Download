bool WebGLRenderingContextBase::ValidateCanvasRenderingContextHost(
    const SecurityOrigin* security_origin,
    const char* function_name,
    CanvasRenderingContextHost* context_host,
    ExceptionState& exception_state) {
  if (!context_host || !context_host->IsPaintable()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "no canvas");
    return false;
  }

  if (WouldTaintOrigin(context_host)) {
    exception_state.ThrowSecurityError("Tainted canvases may not be loaded.");
    return false;
  }
  return true;
}
