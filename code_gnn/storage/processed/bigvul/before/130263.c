bool OSExchangeDataProviderWin::GetHtml(base::string16* html,
                                        GURL* base_url) const {
  std::string url;
  bool success = ClipboardUtil::GetHtml(source_object_, html, &url);
  if (success)
    *base_url = GURL(url);
  return success;
}
