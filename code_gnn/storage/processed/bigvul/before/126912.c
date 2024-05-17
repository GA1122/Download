bool BrowserTabStripController::IsNewTabPage(int model_index) const {
  return model_->ContainsIndex(model_index) &&
      model_->GetTabContentsAt(model_index)->web_contents()->GetURL() ==
      GURL(chrome::kChromeUINewTabURL);
}
