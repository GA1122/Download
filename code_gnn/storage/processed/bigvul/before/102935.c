void DefaultTabHandler::TabPinnedStateChanged(TabContentsWrapper* contents,
                                              int index) {
  delegate_->AsBrowser()->TabPinnedStateChanged(contents, index);
}
