bool OSExchangeDataProviderWin::HasFile() const {
  return ClipboardUtil::HasFilenames(source_object_);
}
