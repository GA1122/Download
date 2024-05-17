void ChromeClientImpl::DidObserveNonGetFetchFromScript() const {
  if (web_view_->PageImportanceSignals())
    web_view_->PageImportanceSignals()->SetIssuedNonGetFetchFromScript();
}
