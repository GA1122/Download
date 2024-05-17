  void ResetShape() {
    shadow_layer_->SetVisible(true);
    large_shadow_layer_->SetVisible(false);
    SetPaintedLayersVisible(false);
    center_point_.SetPoint(small_size_.width() / 2.f,
                           small_size_.height() / 2.f);
  }
