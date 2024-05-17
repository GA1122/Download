  void SetTransforms(const PaintedShapeTransforms transforms) {
    for (int i = 0; i < PAINTED_SHAPE_COUNT; ++i)
      painted_layers_[i]->SetTransform(transforms[i]);
  }
