void BrowserLauncherItemController::FaviconUpdated() {
  UpdateLauncher(tab_model_->GetActiveTabContents());
}
