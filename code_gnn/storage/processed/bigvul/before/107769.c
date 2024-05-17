void FrameLoader::SaveScrollState() {
  if (!document_loader_ || !document_loader_->GetHistoryItem() ||
      !frame_->View())
    return;

  if (NeedsHistoryItemRestore(document_loader_->LoadType()) &&
      !document_loader_->GetInitialScrollState().was_scrolled_by_user)
    return;

  HistoryItem* history_item = document_loader_->GetHistoryItem();
  history_item->SetScrollAnchorData(ScrollAnchorData());
  if (ScrollableArea* layout_scrollable_area = frame_->View()->LayoutViewport())
    history_item->SetScrollOffset(layout_scrollable_area->GetScrollOffset());
  history_item->SetVisualViewportScrollOffset(ToScrollOffset(
      frame_->GetPage()->GetVisualViewport().VisibleRect().Location()));

  if (frame_->IsMainFrame())
    history_item->SetPageScaleFactor(frame_->GetPage()->PageScaleFactor());

  Client()->DidUpdateCurrentHistoryItem();
}
