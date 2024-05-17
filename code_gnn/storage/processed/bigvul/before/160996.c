void ChromeClientImpl::SetBrowserControlsShownRatio(float ratio) {
  web_view_->GetBrowserControls().SetShownRatio(ratio);
  web_view_->DidUpdateBrowserControls();
}
