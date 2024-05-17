String HTMLMediaElement::EffectivePreload() const {
  return PreloadTypeToString(EffectivePreloadType());
}
