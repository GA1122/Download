bool OSExchangeDataProviderWin::HasPlainTextURL(IDataObject* source) {
  base::string16 plain_text;
  return (ClipboardUtil::GetPlainText(source, &plain_text) &&
          !plain_text.empty() && GURL(plain_text).is_valid());
}
