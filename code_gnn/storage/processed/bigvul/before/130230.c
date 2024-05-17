void ClipboardUtil::CFHtmlToHtml(const std::string& cf_html,
                                 std::string* html,
                                 std::string* base_url) {
  size_t fragment_start = std::string::npos;
  size_t fragment_end = std::string::npos;

  ClipboardUtil::CFHtmlExtractMetadata(
      cf_html, base_url, NULL, &fragment_start, &fragment_end);

  if (html &&
      fragment_start != std::string::npos &&
      fragment_end != std::string::npos) {
    *html = cf_html.substr(fragment_start, fragment_end - fragment_start);
    base::TrimWhitespace(*html, base::TRIM_ALL, html);
  }
}
