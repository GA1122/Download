PaintArtifactCompositor::ScrollTranslationForScrollHitTestLayer(
    const PaintArtifact& paint_artifact,
    const PendingLayer& pending_layer) {
  auto paint_chunks =
      paint_artifact.GetPaintChunkSubset(pending_layer.paint_chunk_indices);
  DCHECK(paint_chunks.size());
  const auto& first_paint_chunk = paint_chunks[0];
  if (first_paint_chunk.size() != 1)
    return nullptr;

  const auto& display_item =
      paint_artifact.GetDisplayItemList()[first_paint_chunk.begin_index];
  if (!display_item.IsScrollHitTest())
    return nullptr;

  const auto& scroll_hit_test_display_item =
      static_cast<const ScrollHitTestDisplayItem&>(display_item);
  return &scroll_hit_test_display_item.scroll_offset_node();
}
