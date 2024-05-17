void FrameLoader::RestoreScrollPositionAndViewState() {
  if (!frame_->GetPage() || !GetDocumentLoader() ||
      !GetDocumentLoader()->GetHistoryItem())
    return;
  RestoreScrollPositionAndViewState(
      GetDocumentLoader()->LoadType(), kHistoryDifferentDocumentLoad,
      GetDocumentLoader()->GetHistoryItem()->GetViewState(),
      GetDocumentLoader()->GetHistoryItem()->ScrollRestorationType());
}
