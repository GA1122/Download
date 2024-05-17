void ChunkToLayerMapper::AdjustVisualRectBySubpixelOffset(
    FloatRect& rect) const {
  if (chunk_state_.Transform() == layer_state_.Transform())
    rect.Move(visual_rect_subpixel_offset_);
}
