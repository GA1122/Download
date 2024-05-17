void Document::ClearImportsController() {
  if (!Loader())
    fetcher_->ClearContext();
  imports_controller_ = nullptr;
}
