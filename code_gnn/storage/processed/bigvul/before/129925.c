DomDistillerViewerSource::RequestViewerHandle::~RequestViewerHandle() {
  distilled_page_prefs_->RemoveObserver(this);
}
