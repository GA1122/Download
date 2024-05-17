void StatusBubbleGtk::SetURL(const GURL& url, const string16& languages) {
  url_ = url;
  languages_ = languages;

  if (url.is_empty() && !status_text_.empty()) {
    url_text_ = std::string();
    SetStatusTextTo(status_text_);
    return;
  }

  SetStatusTextToURL();
}
