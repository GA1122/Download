TabContentsWrapper* DefaultTabHandler::AddBlankTabAt(int index,
                                                     bool foreground) {
  return delegate_->AsBrowser()->AddBlankTabAt(index, foreground);
}
