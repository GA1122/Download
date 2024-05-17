String LocalFrame::GetLayerTreeAsTextForTesting(unsigned flags) const {
  if (!ContentLayoutObject())
    return String();

  std::unique_ptr<JSONObject> layers;
  if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled()) {
    layers = View()->CompositedLayersAsJSON(static_cast<LayerTreeFlags>(flags));
  } else {
    if (const auto* root_layer =
            ContentLayoutObject()->Compositor()->RootGraphicsLayer()) {
      if (flags & kLayerTreeIncludesRootLayer && IsMainFrame()) {
        while (root_layer->Parent())
          root_layer = root_layer->Parent();
      }
      layers = root_layer->LayerTreeAsJSON(static_cast<LayerTreeFlags>(flags));
    }
  }

  if (flags & kLayerTreeIncludesPaintInvalidations) {
    std::unique_ptr<JSONArray> object_paint_invalidations =
        view_->TrackedObjectPaintInvalidationsAsJSON();
    if (object_paint_invalidations && object_paint_invalidations->size()) {
      if (!layers)
        layers = JSONObject::Create();
      layers->SetArray("objectPaintInvalidations",
                       std::move(object_paint_invalidations));
    }
  }

  return layers ? layers->ToPrettyJSONString() : String();
}
