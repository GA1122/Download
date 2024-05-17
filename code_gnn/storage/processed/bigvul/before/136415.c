PaintChunk::Id DefaultId() {
  DEFINE_STATIC_LOCAL(FakeDisplayItemClient, fake_client, ());
  return PaintChunk::Id(fake_client, DisplayItem::kDrawingFirst);
}
