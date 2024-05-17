void PaintController::AppendDebugDrawingAfterCommit(
    const DisplayItemClient& display_item_client,
    sk_sp<const PaintRecord> record,
    const PropertyTreeState* property_tree_state) {
  DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
  DCHECK(new_display_item_list_.IsEmpty());
  auto& display_item_list = current_paint_artifact_.GetDisplayItemList();
  auto& display_item =
      display_item_list.AllocateAndConstruct<DrawingDisplayItem>(
          display_item_client, DisplayItem::kDebugDrawing, std::move(record));
  display_item.SetSkippedCache();

  if (property_tree_state) {
    DCHECK(RuntimeEnabledFeatures::SlimmingPaintV175Enabled());
    current_paint_artifact_.PaintChunks().emplace_back(
        display_item_list.size() - 1, display_item_list.size(),
        display_item.GetId(), *property_tree_state);
  }
}
