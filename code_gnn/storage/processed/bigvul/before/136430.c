  cc::Layer* ScrollHitTestLayerAt(unsigned index) {
    return paint_artifact_compositor_->GetExtraDataForTesting()
        ->scroll_hit_test_layers[index]
        .get();
  }
