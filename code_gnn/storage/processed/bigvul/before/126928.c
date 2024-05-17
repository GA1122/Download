void BrowserTabStripController::TabChangedAt(TabContents* contents,
                                             int model_index,
                                             TabChangeType change_type) {
  if (change_type == TITLE_NOT_LOADING) {
    tabstrip_->TabTitleChangedNotLoading(model_index);
    return;
  }

  SetTabDataAt(contents->web_contents(), model_index);
}
