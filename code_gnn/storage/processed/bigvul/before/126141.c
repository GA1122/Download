string16 BrowserLauncherItemController::GetTitle() {
  if (type() == TYPE_TABBED || type() == TYPE_EXTENSION_PANEL) {
    if (tab_model_->GetActiveTabContents()) {
      const content::WebContents* contents =
          tab_model_->GetActiveTabContents()->web_contents();
      if (contents)
        return contents->GetTitle();
    }
  }
  return GetAppTitle();
}
