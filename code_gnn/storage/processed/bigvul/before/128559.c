int ExtractPrintPreviewPageIndex(const std::string& src_url) {
  std::vector<std::string> url_substr;
  base::SplitString(src_url.substr(strlen(kChromePrint)), '/', &url_substr);
  if (url_substr.size() != 3)
    return -1;

  if (url_substr[2] != "print.pdf")
    return -1;

  int page_index = 0;
  if (!base::StringToInt(url_substr[1], &page_index))
    return -1;
  return page_index;
}
