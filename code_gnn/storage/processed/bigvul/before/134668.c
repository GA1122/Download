bool OSExchangeDataProviderWin::HasURL() const {
  return (ClipboardUtil::HasUrl(source_object_) ||
          HasPlainTextURL(source_object_));
}
