void DefaultTabHandler::TabInsertedAt(TabContentsWrapper* contents,
                                      int index,
                                      bool foreground) {
  delegate_->AsBrowser()->TabInsertedAt(contents, index, foreground);
}
