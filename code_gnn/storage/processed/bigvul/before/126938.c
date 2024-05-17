void BrowserTabStripController::UpdateLoadingAnimations() {
  for (int i = 0, tab_count = tabstrip_->tab_count(); i < tab_count; ++i) {
    if (model_->ContainsIndex(i)) {
      Tab* tab = tabstrip_->tab_at(i);
      TabContents* contents = model_->GetTabContentsAt(i);
      tab->UpdateLoadingAnimation(
          TabContentsNetworkState(contents->web_contents()));
    }
  }
}
