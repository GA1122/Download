const char* const* WebGLRenderingContextBase::ExtensionTracker::Prefixes()
    const {
  static const char* const kUnprefixed[] = {
      "", nullptr,
  };
  return prefixes_ ? prefixes_ : kUnprefixed;
}
