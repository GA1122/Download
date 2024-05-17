bool Document::CheckCompletedInternal() {
  if (!ShouldComplete())
    return false;

  if (frame_) {
    frame_->Client()->RunScriptsAtDocumentIdle();

    if (!frame_)
      return false;

    if (!ShouldComplete())
      return false;
  }

  SetReadyState(kComplete);
  if (LoadEventStillNeeded())
    ImplicitClose();

  if (!frame_ || !frame_->IsAttached())
    return false;
  frame_->GetNavigationScheduler().StartTimer();
  View()->HandleLoadCompleted();
  if (!AllDescendantsAreComplete(frame_))
    return false;

  if (!Loader()->SentDidFinishLoad()) {
    if (frame_->IsMainFrame())
      GetViewportData().GetViewportDescription().ReportMobilePageStats(frame_);
    Loader()->SetSentDidFinishLoad();
    frame_->Client()->DispatchDidFinishLoad();
    if (!frame_)
      return false;

    if (frame_->Client()->GetRemoteNavigationAssociatedInterfaces()) {
      mojom::blink::UkmSourceIdFrameHostAssociatedPtr ukm_binding;
      frame_->Client()->GetRemoteNavigationAssociatedInterfaces()->GetInterface(
          &ukm_binding);
      DCHECK(ukm_binding.is_bound());
      ukm_binding->SetDocumentSourceId(ukm_source_id_);
    }

    AnchorElementMetrics::MaybeReportViewportMetricsOnLoad(*this);

    PreviewsResourceLoadingHints* hints =
        Loader()->GetPreviewsResourceLoadingHints();
    if (hints) {
      hints->RecordUKM(UkmRecorder());
    }
  }

  return true;
}
