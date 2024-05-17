 TestPaintArtifact& TestPaintArtifact::Chunk(
    const PropertyTreeState& properties) {
  return Chunk(NewClient(), properties);
}
