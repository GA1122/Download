KURL Document::CompleteURL(const String& url) const {
  return CompleteURLWithOverride(url, base_url_);
}
