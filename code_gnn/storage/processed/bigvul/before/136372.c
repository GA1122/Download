  CompositedLayerRasterInvalidatorTest& Bounds(const FloatRect& bounds) {
    data_.chunks.back().bounds = bounds;
    return *this;
  }
