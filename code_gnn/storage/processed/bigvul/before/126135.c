void BrowserLauncherItemController::BrowserActivationStateChanged() {
  content::WebContents* active_contents = tab_model_->GetActiveWebContents();
  if (active_contents)
    UpdateAppState(active_contents);
  UpdateItemStatus();
}
