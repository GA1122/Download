bool FrameLoader::ShouldSerializeScrollAnchor() {
  return RuntimeEnabledFeatures::ScrollAnchorSerializationEnabled();
}
