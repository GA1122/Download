void CompositedLayerRasterInvalidator::Generate(
    const PaintArtifact& paint_artifact,
    const PaintChunkSubset& paint_chunks,
    const gfx::Rect& layer_bounds,
    const PropertyTreeState& layer_state,
    const FloatSize& visual_rect_subpixel_offset) {
  if (RuntimeEnabledFeatures::DisableRasterInvalidationEnabled())
    return;

  if (RasterInvalidationTracking::ShouldAlwaysTrack())
    EnsureTracking();

  if (tracking_info_) {
    for (const auto& chunk : paint_chunks) {
      tracking_info_->new_client_debug_names.insert(
          &chunk.id.client, chunk.id.client.DebugName());
    }
  }

  bool layer_bounds_was_empty = layer_bounds_.IsEmpty();
  layer_bounds_ = layer_bounds;

  Vector<PaintChunkInfo> new_chunks_info;
  new_chunks_info.ReserveCapacity(paint_chunks.size());

  if (layer_bounds_was_empty || layer_bounds_.IsEmpty()) {
    ChunkToLayerMapper mapper(layer_state, layer_bounds.OffsetFromOrigin(),
                              visual_rect_subpixel_offset);
    for (const auto& chunk : paint_chunks) {
      mapper.SwitchToChunk(chunk);
      new_chunks_info.emplace_back(*this, mapper, chunk);
    }
  } else {
    GenerateRasterInvalidations(paint_artifact, paint_chunks, layer_state,
                                visual_rect_subpixel_offset, new_chunks_info);
  }

  paint_chunks_info_ = std::move(new_chunks_info);

  if (tracking_info_) {
    tracking_info_->old_client_debug_names =
        std::move(tracking_info_->new_client_debug_names);
  }
}
