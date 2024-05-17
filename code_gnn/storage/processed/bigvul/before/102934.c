void DefaultTabHandler::TabMoved(TabContentsWrapper* contents,
                                 int from_index,
                                 int to_index) {
  delegate_->AsBrowser()->TabMoved(contents, from_index, to_index);
}
