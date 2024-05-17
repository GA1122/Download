TestPaintArtifact& TestPaintArtifact::RectDrawing(DisplayItemClient& client,
                                                  const FloatRect& bounds,
                                                  Color color) {
  display_item_list_.AllocateAndConstruct<DrawingDisplayItem>(
      client, DisplayItem::kDrawingFirst,
      static_cast<DummyRectClient&>(client).MakeRecord(bounds, color));
  return *this;
}
