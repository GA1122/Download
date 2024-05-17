void CompositedLayerRasterInvalidator::AddDisplayItemRasterInvalidations(
    const PaintArtifact& paint_artifact,
    const PaintChunk& chunk,
    const ChunkToLayerMapper& mapper) {
  const auto* rects = paint_artifact.GetRasterInvalidationRects(chunk);
  if (!rects || rects->IsEmpty())
    return;

  const auto* tracking = paint_artifact.GetRasterInvalidationTracking(chunk);
  DCHECK(!tracking || tracking->IsEmpty() || tracking->size() == rects->size());

  for (size_t i = 0; i < rects->size(); ++i) {
    auto rect = ClipByLayerBounds(mapper.MapVisualRect((*rects)[i]));
    if (rect.IsEmpty())
      continue;
    raster_invalidation_function_(rect);

    if (tracking && !tracking->IsEmpty()) {
      const auto& info = (*tracking)[i];
      tracking_info_->tracking.AddInvalidation(
          info.client, info.client_debug_name, rect, info.reason);
    }
  }
}
