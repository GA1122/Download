void DefaultTabHandler::TabClosingAt(TabStripModel* tab_strip_model,
                                     TabContentsWrapper* contents,
                                     int index) {
  delegate_->AsBrowser()->TabClosingAt(tab_strip_model, contents, index);
}
