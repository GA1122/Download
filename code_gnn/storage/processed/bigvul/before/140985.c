void Document::ClearImportsController() {
  fetcher_->ClearContext();
  imports_controller_ = nullptr;
}
