bool WebGLRenderingContextBase::ValidateHTMLVideoElement(
    const SecurityOrigin* security_origin,
    const char* function_name,
    HTMLVideoElement* video,
    ExceptionState& exception_state) {
  if (!video || !video->videoWidth() || !video->videoHeight()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "no video");
    return false;
  }

  if (WouldTaintOrigin(video, security_origin)) {
    exception_state.ThrowSecurityError(
        "The video element contains cross-origin data, and may not be loaded.");
    return false;
  }
  return true;
}
