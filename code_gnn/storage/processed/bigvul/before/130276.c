bool OSExchangeDataProviderWin::HasHtml() const {
  return ClipboardUtil::HasHtml(source_object_);
}
