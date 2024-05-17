String HTMLMediaElement::effectivePreload() const {
  return preloadTypeToString(effectivePreloadType());
}
