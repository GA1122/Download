void FrameLoader::ClearInitialScrollState() {
  document_loader_->GetInitialScrollState().was_scrolled_by_user = false;
}
