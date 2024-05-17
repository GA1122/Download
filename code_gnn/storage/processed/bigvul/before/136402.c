void PaintArtifactCompositor::ShowDebugData() {
  LOG(ERROR) << LayersAsJSON(kLayerTreeIncludesDebugInfo |
                             kLayerTreeIncludesPaintInvalidations)
                    ->ToPrettyJSONString()
                    .Utf8()
                    .data();
}
