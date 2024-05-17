bool OSExchangeDataProviderWin::HasString() const {
  return ClipboardUtil::HasPlainText(source_object_);
}
