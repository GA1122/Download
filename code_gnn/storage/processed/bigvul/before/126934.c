void BrowserTabStripController::TabReplacedAt(TabStripModel* tab_strip_model,
                                              TabContents* old_contents,
                                              TabContents* new_contents,
                                              int model_index) {
  SetTabDataAt(new_contents->web_contents(), model_index);
}
