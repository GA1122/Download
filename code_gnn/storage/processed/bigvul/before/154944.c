bool WebGLRenderingContextBase::ExtensionSupportedAndAllowed(
    const ExtensionTracker* tracker) {
  if (tracker->Draft() &&
      !RuntimeEnabledFeatures::WebGLDraftExtensionsEnabled())
    return false;
  if (!tracker->Supported(this))
    return false;
  if (disabled_extensions_.Contains(String(tracker->ExtensionName())))
    return false;
  return true;
}
