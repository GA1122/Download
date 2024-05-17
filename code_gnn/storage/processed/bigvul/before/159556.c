void Document::CheckCompleted() {
  if (!ShouldComplete())
    return;

  if (frame_) {
    frame_->Client()->RunScriptsAtDocumentIdle();

    if (!frame_)
      return;

    if (!ShouldComplete())
      return;
  }

  SetReadyState(kComplete);
  if (LoadEventStillNeeded())
    ImplicitClose();

  if (!frame_ || !frame_->IsAttached())
    return;
  if (frame_->GetSettings()->GetSavePreviousDocumentResources() ==
      SavePreviousDocumentResources::kUntilOnLoad) {
    fetcher_->ClearResourcesFromPreviousFetcher();
  }
  frame_->GetNavigationScheduler().StartTimer();
  View()->HandleLoadCompleted();
  if (!AllDescendantsAreComplete(frame_))
    return;

  if (!Loader()->SentDidFinishLoad()) {
    if (frame_->IsMainFrame())
      GetViewportDescription().ReportMobilePageStats(frame_);
    Loader()->SetSentDidFinishLoad();
    frame_->Client()->DispatchDidFinishLoad();
    if (!frame_)
      return;
  }

  frame_->Loader().DidFinishNavigation();
}
