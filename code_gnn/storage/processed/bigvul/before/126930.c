void BrowserTabStripController::TabInsertedAt(WebContents* contents,
                                              int model_index,
                                              bool is_active) {
  DCHECK(contents);
  DCHECK(model_->ContainsIndex(model_index));
   AddTab(contents, model_index, is_active);
 }
