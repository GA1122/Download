  static PaintChunk Chunk(const PropertyTreeState& state) {
    DEFINE_STATIC_LOCAL(FakeDisplayItemClient, fake_client, ());
    DEFINE_STATIC_LOCAL(
        base::Optional<PaintChunk::Id>, id,
        (PaintChunk::Id(fake_client, DisplayItem::kDrawingFirst)));
    PaintChunk chunk(0, 0, *id, state);
    return chunk;
  }
