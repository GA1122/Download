void FrameLoader::SetDefersLoading(bool defers) {
  if (frame_->GetDocument())
    frame_->GetDocument()->Fetcher()->SetDefersLoading(defers);
  if (document_loader_)
    document_loader_->SetDefersLoading(defers);
  if (provisional_document_loader_)
    provisional_document_loader_->SetDefersLoading(defers);
  if (!defers)
    frame_->GetNavigationScheduler().StartTimer();
}
