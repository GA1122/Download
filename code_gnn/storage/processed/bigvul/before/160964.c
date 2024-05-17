bool ChromeClientImpl::HadFormInteraction() const {
  return web_view_->PageImportanceSignals() &&
         web_view_->PageImportanceSignals()->HadFormInteraction();
}
