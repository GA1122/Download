 void BrowserLauncherItemController::TabChangedAt(
    TabContents* tab,
    int index,
    TabStripModelObserver::TabChangeType change_type) {
  UpdateAppState(tab->web_contents());
  if (index != tab_model_->active_index() ||
      !(change_type != TabStripModelObserver::LOADING_ONLY &&
        change_type != TabStripModelObserver::TITLE_NOT_LOADING)) {
    return;
  }

  UpdateLauncher(tab);
}
