void FrameLoader::MarkAsLoading() {
  DCHECK(!provisional_document_loader_);
  DCHECK(frame_->GetDocument()->IsLoadCompleted());
  DCHECK(frame_->GetDocument()->HasFinishedParsing());
  progress_tracker_->ProgressStarted();
}
