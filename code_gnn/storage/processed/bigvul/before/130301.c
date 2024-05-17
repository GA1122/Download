void OSExchangeDataProviderWin::SetHtml(const base::string16& html,
                                        const GURL& base_url) {
  std::string utf8_html = base::UTF16ToUTF8(html);
  std::string url = base_url.is_valid() ? base_url.spec() : std::string();

  std::string cf_html = ClipboardUtil::HtmlToCFHtml(utf8_html, url);
  STGMEDIUM* storage = GetStorageForBytes(cf_html.c_str(), cf_html.size());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetHtmlFormatType().ToFormatEtc(), storage));

  STGMEDIUM* storage_plain = GetStorageForBytes(utf8_html.c_str(),
                                                utf8_html.size());
  data_->contents_.push_back(new DataObjectImpl::StoredDataInfo(
      Clipboard::GetTextHtmlFormatType().ToFormatEtc(), storage_plain));
}
