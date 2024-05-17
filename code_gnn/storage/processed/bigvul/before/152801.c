  cc::Layer* ContentLayerAt(size_t index) {
    return paint_artifact_compositor()
        ->GetExtraDataForTesting()
        ->content_layers[index]
        .get();
  }
