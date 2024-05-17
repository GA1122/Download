void DefaultTabHandler::TabSelectedAt(TabContentsWrapper* old_contents,
                                      TabContentsWrapper* new_contents,
                                      int index,
                                      bool user_gesture) {
  delegate_->AsBrowser()->TabSelectedAt(old_contents,
                                        new_contents,
                                        index,
                                        user_gesture);
}
