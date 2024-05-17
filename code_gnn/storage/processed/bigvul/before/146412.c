bool WebGLRenderingContextBase::ValidateImageBitmap(
    const char* function_name,
    ImageBitmap* bitmap,
    ExceptionState& exception_state) {
  if (bitmap->IsNeutered()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name,
                      "The source data has been detached.");
    return false;
  }
  if (!bitmap->OriginClean()) {
    exception_state.ThrowSecurityError(
        "The ImageBitmap contains cross-origin data, and may not be loaded.");
    return false;
  }
  return true;
}
