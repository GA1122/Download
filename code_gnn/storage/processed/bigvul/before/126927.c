void BrowserTabStripController::TabBlockedStateChanged(WebContents* contents,
                                                       int model_index) {
  SetTabDataAt(contents, model_index);
}