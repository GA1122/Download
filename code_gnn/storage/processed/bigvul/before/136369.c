void CompositedLayerRasterInvalidator::IncrementallyInvalidateChunk(
    const PaintChunkInfo& old_chunk,
    const PaintChunkInfo& new_chunk) {
  SkRegion diff(old_chunk.bounds_in_layer);
  diff.op(new_chunk.bounds_in_layer, SkRegion::kXOR_Op);
  for (SkRegion::Iterator it(diff); !it.done(); it.next()) {
    const SkIRect& r = it.rect();
    AddRasterInvalidation(IntRect(r.x(), r.y(), r.width(), r.height()),
                          &new_chunk.id.client,
                          PaintInvalidationReason::kIncremental);
  }
}
