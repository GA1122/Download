void DomDistillerViewerSource::RequestViewerHandle::WebContentsDestroyed() {
  Cancel();
}
