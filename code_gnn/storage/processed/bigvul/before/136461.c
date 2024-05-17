 PaintChunk::Id DefaultId() {
   DEFINE_STATIC_LOCAL(FakeDisplayItemClient, fake_client,
                       ("FakeDisplayItemClient", LayoutRect(0, 0, 100, 100)));
  return PaintChunk::Id(fake_client, DisplayItem::kDrawingFirst);
}
