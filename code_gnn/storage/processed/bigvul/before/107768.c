void FrameLoader::SaveScrollAnchor() {
  if (!RuntimeEnabledFeatures::ScrollAnchorSerializationEnabled())
    return;

  if (!document_loader_ || !document_loader_->GetHistoryItem() ||
      !frame_->View())
    return;

  if (NeedsHistoryItemRestore(document_loader_->LoadType()) &&
      !document_loader_->GetInitialScrollState().was_scrolled_by_user)
    return;

  HistoryItem* history_item = document_loader_->GetHistoryItem();
  if (ScrollableArea* layout_scrollable_area =
          frame_->View()->LayoutViewport()) {
    ScrollAnchor* scroll_anchor = layout_scrollable_area->GetScrollAnchor();
    DCHECK(scroll_anchor);

    const SerializedAnchor& serialized_anchor =
        scroll_anchor->GetSerializedAnchor();
    if (serialized_anchor.IsValid()) {
      history_item->SetScrollAnchorData(
          {serialized_anchor.selector,
           WebFloatPoint(serialized_anchor.relative_offset.X(),
                         serialized_anchor.relative_offset.Y()),
           serialized_anchor.simhash});
    }
  }
}
