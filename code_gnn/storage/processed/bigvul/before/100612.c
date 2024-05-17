  void SetPaintedLayersVisible(bool visible) {
    for (int i = 0; i < PAINTED_SHAPE_COUNT; ++i)
      painted_layers_[i]->SetVisible(visible);
  }
