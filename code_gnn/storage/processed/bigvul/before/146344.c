const char* const* WebGLRenderingContextBase::ExtensionTracker::Prefixes()
    const {
  static const char* const kUnprefixed[] = {
      "", 0,
  };
  return prefixes_ ? prefixes_ : kUnprefixed;
}
