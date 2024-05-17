  TestPaintArtifact& TestPaintArtifact::ScrollHitTest(
      DisplayItemClient& client,
    scoped_refptr<const TransformPaintPropertyNode> scroll_offset) {
//     const TransformPaintPropertyNode& scroll_offset) {
    display_item_list_.AllocateAndConstruct<ScrollHitTestDisplayItem>(
      client, DisplayItem::kScrollHitTest, std::move(scroll_offset));
//       client, DisplayItem::kScrollHitTest, scroll_offset);
    return *this;
  }