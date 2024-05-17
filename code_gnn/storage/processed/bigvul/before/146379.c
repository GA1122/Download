bool WebGLRenderingContextBase::SupportOwnOffscreenSurface(
    ExecutionContext* execution_context) {
  if (RuntimeEnabledFeatures::WebVRExperimentalRenderingEnabled()) {
    if (RuntimeEnabledFeatures::WebVREnabled() ||
        OriginTrials::webVREnabled(execution_context)) {
      DVLOG(1) << "Requesting supportOwnOffscreenSurface";
      return true;
    }
  }
  return false;
}
