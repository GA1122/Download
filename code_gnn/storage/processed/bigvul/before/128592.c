bool IsPrintPreviewUrl(const std::string& url) {
  return url.substr(0, strlen(kChromePrint)) == kChromePrint;
}
