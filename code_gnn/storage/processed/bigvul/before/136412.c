  cc::Layer* ContentLayerAt(unsigned index) {
    return paint_artifact_compositor_->GetExtraDataForTesting()
        ->content_layers[index]
        .get();
  }
