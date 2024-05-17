bool WebGLRenderingContextBase::ValidateHTMLCanvasElement(
    SecurityOrigin* security_origin,
    const char* function_name,
    HTMLCanvasElement* canvas,
    ExceptionState& exception_state) {
  if (!canvas || !canvas->IsPaintable()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "no canvas");
    return false;
  }

  if (WouldTaintOrigin(canvas, security_origin)) {
    exception_state.ThrowSecurityError("Tainted canvases may not be loaded.");
    return false;
  }
  return true;
}
