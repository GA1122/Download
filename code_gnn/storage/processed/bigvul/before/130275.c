bool OSExchangeDataProviderWin::HasFileContents() const {
  return ClipboardUtil::HasFileContents(source_object_);
}
