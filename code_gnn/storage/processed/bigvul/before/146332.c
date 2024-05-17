bool WebGLRenderingContextBase::ExtensionTracker::MatchesNameWithPrefixes(
    const String& name) const {
  const char* const* prefix_set = Prefixes();
  for (; *prefix_set; ++prefix_set) {
    String prefixed_name = String(*prefix_set) + ExtensionName();
    if (DeprecatedEqualIgnoringCase(prefixed_name, name)) {
      return true;
    }
  }
  return false;
}
