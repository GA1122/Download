void FrameLoader::RestoreScrollPositionAndViewState() {
  if (!frame_->GetPage() || !GetDocumentLoader() ||
      !GetDocumentLoader()->GetHistoryItem() ||
      !GetDocumentLoader()->GetHistoryItem()->GetViewState() ||
      in_restore_scroll_) {
    return;
  }
  base::AutoReset<bool> in_restore_scroll(&in_restore_scroll_, true);
  RestoreScrollPositionAndViewState(
      GetDocumentLoader()->LoadType(), false  ,
      *GetDocumentLoader()->GetHistoryItem()->GetViewState(),
      GetDocumentLoader()->GetHistoryItem()->ScrollRestorationType());
}
