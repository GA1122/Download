WebGLRenderingContextBase::getSupportedExtensions() {
  if (isContextLost())
    return base::nullopt;

  Vector<String> result;

  for (size_t i = 0; i < extensions_.size(); ++i) {
    ExtensionTracker* tracker = extensions_[i].Get();
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
