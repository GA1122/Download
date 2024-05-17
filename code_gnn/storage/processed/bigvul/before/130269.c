bool OSExchangeDataProviderWin::GetPlainTextURL(IDataObject* source,
                                                GURL* url) {
  base::string16 plain_text;
  if (ClipboardUtil::GetPlainText(source, &plain_text) &&
      !plain_text.empty()) {
    GURL gurl(plain_text);
    if (gurl.is_valid()) {
      *url = gurl;
      return true;
    }
  }
  return false;
}
