TestPaintArtifact& TestPaintArtifact::ForeignLayer(
    DisplayItemClient& client,
    const FloatPoint& location,
    const IntSize& size,
    scoped_refptr<cc::Layer> layer) {
  static_cast<DummyRectClient&>(client).SetVisualRect(
      FloatRect(location, FloatSize(size)));
  display_item_list_.AllocateAndConstruct<ForeignLayerDisplayItem>(
      client, DisplayItem::kForeignLayerFirst, std::move(layer), location,
      size);
  return *this;
 }
