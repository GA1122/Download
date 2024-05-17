IntRect ChunkToLayerMapper::MapVisualRect(const FloatRect& rect) const {
  if (rect.IsEmpty())
    return IntRect();

  if (UNLIKELY(has_filter_that_moves_pixels_))
    return MapUsingGeometryMapper(rect);

  FloatRect mapped_rect = transform_.MapRect(rect);
  if (!mapped_rect.IsEmpty() && !clip_rect_.IsInfinite())
    mapped_rect.Intersect(clip_rect_.Rect());

  if (mapped_rect.IsEmpty()) {
    DCHECK(MapUsingGeometryMapper(rect).IsEmpty());
    return IntRect();
  }

  mapped_rect.Inflate(outset_for_raster_effects_);
  AdjustVisualRectBySubpixelOffset(mapped_rect);

  auto result = EnclosingIntRect(mapped_rect);
#if DCHECK_IS_ON()
  auto slow_result = MapUsingGeometryMapper(rect);
  if (result != slow_result) {
    LOG(WARNING) << "ChunkToLayerMapper::MapVisualRect: Different results from"
                 << "fast path (" << result << ") and slow path ("
                 << slow_result << ")";
  }
#endif
  return result;
}
