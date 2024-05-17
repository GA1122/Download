bool WebGLRenderingContextBase::ValidateHTMLImageElement(
    const SecurityOrigin* security_origin,
    const char* function_name,
    HTMLImageElement* image,
    ExceptionState& exception_state) {
  if (!image || !image->CachedImage()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "no image");
    return false;
  }
  const KURL& url = image->CachedImage()->GetResponse().CurrentRequestUrl();
  if (url.IsNull() || url.IsEmpty() || !url.IsValid()) {
    SynthesizeGLError(GL_INVALID_VALUE, function_name, "invalid image");
    return false;
  }

  if (WouldTaintOrigin(image)) {
    exception_state.ThrowSecurityError(
        "The image element contains cross-origin data, and may not be loaded.");
    return false;
  }
  return true;
}