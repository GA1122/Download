static std::string SniffMimeType(const std::string& content,
                                 const std::string& url,
                                 const std::string& mime_type_hint) {
  std::string mime_type;
  SniffMimeType(content.data(), content.size(), GURL(url),
                     mime_type_hint, &mime_type);
  return mime_type;
}
