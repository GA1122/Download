void CompositedLayerRasterInvalidator::GenerateRasterInvalidations(
    const PaintArtifact& paint_artifact,
    const PaintChunkSubset& new_chunks,
    const PropertyTreeState& layer_state,
    const FloatSize& visual_rect_subpixel_offset,
    Vector<PaintChunkInfo>& new_chunks_info) {
  ChunkToLayerMapper mapper(layer_state, layer_bounds_.OffsetFromOrigin(),
                            visual_rect_subpixel_offset);
  Vector<bool> old_chunks_matched;
  old_chunks_matched.resize(paint_chunks_info_.size());
  size_t old_index = 0;
  size_t max_matched_old_index = 0;
  for (const auto& new_chunk : new_chunks) {
    mapper.SwitchToChunk(new_chunk);
    auto& new_chunk_info =
        new_chunks_info.emplace_back(*this, mapper, new_chunk);

    if (!new_chunk.is_cacheable) {
      FullyInvalidateNewChunk(new_chunk_info,
                              PaintInvalidationReason::kChunkUncacheable);
      continue;
    }

    size_t matched_old_index = MatchNewChunkToOldChunk(new_chunk, old_index);
    if (matched_old_index == kNotFound) {
      FullyInvalidateNewChunk(new_chunk_info,
                              PaintInvalidationReason::kChunkAppeared);
      continue;
    }

    DCHECK(!old_chunks_matched[matched_old_index]);
    old_chunks_matched[matched_old_index] = true;

    auto& old_chunk_info = paint_chunks_info_[matched_old_index];
    old_chunk_info.bounds_in_layer =
        ClipByLayerBounds(old_chunk_info.bounds_in_layer);

    PaintInvalidationReason reason =
        matched_old_index < max_matched_old_index
            ? PaintInvalidationReason::kChunkReordered
            : ChunkPropertiesChanged(new_chunk.properties, new_chunk_info,
                                     old_chunk_info, layer_state);

    if (IsFullPaintInvalidationReason(reason)) {
      FullyInvalidateChunk(old_chunk_info, new_chunk_info, reason);
    } else {
      new_chunk_info.chunk_to_layer_transform =
          old_chunk_info.chunk_to_layer_transform;

      if (reason == PaintInvalidationReason::kIncremental)
        IncrementallyInvalidateChunk(old_chunk_info, new_chunk_info);

      AddDisplayItemRasterInvalidations(paint_artifact, new_chunk, mapper);
    }

    old_index = matched_old_index + 1;
    if (old_index == paint_chunks_info_.size())
      old_index = 0;
    max_matched_old_index = std::max(max_matched_old_index, matched_old_index);
  }

  for (size_t i = 0; i < paint_chunks_info_.size(); ++i) {
    if (old_chunks_matched[i])
      continue;
    FullyInvalidateOldChunk(paint_chunks_info_[i],
                            paint_chunks_info_[i].is_cacheable
                                ? PaintInvalidationReason::kChunkDisappeared
                                : PaintInvalidationReason::kChunkUncacheable);
  }
}
