TestPaintArtifact& TestPaintArtifact::KnownToBeOpaque() {
  paint_chunks_data_.chunks.back().known_to_be_opaque = true;
  return *this;
}
