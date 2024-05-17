void FrameLoader::SetDefersLoading(bool defers) {
  if (provisional_document_loader_)
    provisional_document_loader_->Fetcher()->SetDefersLoading(defers);

  if (Document* document = frame_->GetDocument()) {
    document->Fetcher()->SetDefersLoading(defers);
    if (defers)
      document->PauseScheduledTasks();
    else
      document->UnpauseScheduledTasks();
  }

  if (!defers)
    frame_->GetNavigationScheduler().StartTimer();
}
