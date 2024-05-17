bool FrameLoader::CancelProvisionalLoaderForNewNavigation(
    bool cancel_scheduled_navigations,
    bool is_starting_blank_navigation) {
  bool had_placeholder_client_document_loader =
      provisional_document_loader_ && !provisional_document_loader_->DidStart();

  if (had_placeholder_client_document_loader)
    provisional_document_loader_->SetSentDidFinishLoad();

  frame_->GetDocument()->Abort();
  if (!frame_->GetPage())
    return false;

  DetachDocumentLoader(provisional_document_loader_);
  if (!frame_->GetPage())
    return false;

  progress_tracker_->ProgressStarted();

  bool skip_cancel_for_about_blank =
      state_machine_.CommittedFirstRealDocumentLoad() &&
      is_starting_blank_navigation;
  if (!skip_cancel_for_about_blank &&
      (!had_placeholder_client_document_loader ||
       cancel_scheduled_navigations)) {
    frame_->GetNavigationScheduler().Cancel();
  }

  return true;
}
