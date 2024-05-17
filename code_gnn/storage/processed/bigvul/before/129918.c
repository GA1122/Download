void DomDistillerViewerSource::RequestViewerHandle::RenderProcessGone(
    base::TerminationStatus status) {
  Cancel();
}
