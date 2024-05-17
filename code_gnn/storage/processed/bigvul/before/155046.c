WebGLRenderingContextBase::getSupportedExtensions() {
  if (isContextLost())
    return base::nullopt;

  Vector<String> result;

  for (ExtensionTracker* tracker : extensions_) {
    if (ExtensionSupportedAndAllowed(tracker)) {
      const char* const* prefixes = tracker->Prefixes();
      for (; *prefixes; ++prefixes) {
        String prefixed_name = String(*prefixes) + tracker->ExtensionName();
        result.push_back(prefixed_name);
      }
    }
  }

  return result;
}
