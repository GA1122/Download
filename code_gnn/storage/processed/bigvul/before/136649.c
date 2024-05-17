void FrameLoader::Detach() {
  DetachDocumentLoader(document_loader_);
  DetachDocumentLoader(provisional_document_loader_);

  if (progress_tracker_) {
    progress_tracker_->Dispose();
    progress_tracker_.Clear();
  }

  TRACE_EVENT_OBJECT_DELETED_WITH_ID("loading", "FrameLoader", this);
  detached_ = true;
}
