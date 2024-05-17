size_t CompositedLayerRasterInvalidator::ApproximateUnsharedMemoryUsage()
    const {
  return sizeof(*this) + paint_chunks_info_.capacity() * sizeof(PaintChunkInfo);
}
