void DefaultTabHandler::TabDeselected(TabContentsWrapper* contents) {
  delegate_->AsBrowser()->TabDeselected(contents);
}
