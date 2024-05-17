IntRect ChunkToLayerMapper::MapUsingGeometryMapper(
    const FloatRect& rect) const {
  FloatClipRect visual_rect(rect);
  GeometryMapper::LocalToAncestorVisualRect(chunk_state_, layer_state_,
                                            visual_rect);
  if (visual_rect.Rect().IsEmpty())
    return IntRect();

  visual_rect.Rect().Move(-layer_offset_.x(), -layer_offset_.y());
  visual_rect.Rect().Inflate(outset_for_raster_effects_);
  AdjustVisualRectBySubpixelOffset(visual_rect.Rect());
  return EnclosingIntRect(visual_rect.Rect());
}
