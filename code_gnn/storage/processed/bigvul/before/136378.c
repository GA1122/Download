  CompositedLayerRasterInvalidatorTest& RasterInvalidationCount(int count) {
    size_t size = data_.chunks.size();
    DCHECK_GT(size, 0u);
    data_.raster_invalidation_rects.resize(size);
    data_.raster_invalidation_trackings.resize(size);
    int index = static_cast<int>(size - 1);
    for (int i = 0; i < count; ++i) {
      IntRect rect(index * 11, index * 22, index * 22 + 100 + i,
                   index * 11 + 100 + i);
      data_.raster_invalidation_rects.back().push_back(FloatRect(rect));
      data_.raster_invalidation_trackings.back().push_back(
          RasterInvalidationInfo{
              &data_.chunks.back().id.client, "Test", rect,
              static_cast<PaintInvalidationReason>(
                  static_cast<int>(PaintInvalidationReason::kFull) + index +
                  i)});
    }
    return *this;
  }
