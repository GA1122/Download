  CompositedLayerRasterInvalidatorTest& Chunk(int type) {
    DEFINE_STATIC_LOCAL(FakeDisplayItemClient, fake_client, ());
    fake_client.ClearIsJustCreated();
    PaintChunk::Id id(fake_client, static_cast<DisplayItem::Type>(
                                       DisplayItem::kDrawingFirst + type));
    data_.chunks.emplace_back(0, 0, id, DefaultPropertyTreeState());
    data_.chunks.back().bounds =
        FloatRect(type * 110, type * 220, type * 220 + 200, type * 110 + 200);
    return *this;
   }
