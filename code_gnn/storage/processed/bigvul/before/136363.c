void CompositedLayerRasterInvalidator::FullyInvalidateChunk(
    const PaintChunkInfo& old_chunk,
    const PaintChunkInfo& new_chunk,
    PaintInvalidationReason reason) {
  FullyInvalidateOldChunk(old_chunk, reason);
  if (old_chunk.bounds_in_layer != new_chunk.bounds_in_layer)
    FullyInvalidateNewChunk(new_chunk, reason);
}
