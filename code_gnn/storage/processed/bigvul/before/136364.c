void CompositedLayerRasterInvalidator::FullyInvalidateNewChunk(
    const PaintChunkInfo& info,
    PaintInvalidationReason reason) {
  AddRasterInvalidation(info.bounds_in_layer, &info.id.client, reason);
}
