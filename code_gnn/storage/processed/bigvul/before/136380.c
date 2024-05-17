  CompositedLayerRasterInvalidatorTest& Uncacheable() {
    data_.chunks.back().is_cacheable = false;
    return *this;
  }
