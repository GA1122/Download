void HttpResponseHeaders::GetMimeTypeAndCharset(std::string* mime_type,
                                                std::string* charset) const {
  mime_type->clear();
  charset->clear();

  std::string name = "content-type";
  std::string value;

  bool had_charset = false;

  void* iter = NULL;
  while (EnumerateHeader(&iter, name, &value))
    HttpUtil::ParseContentType(value, mime_type, charset, &had_charset);
}
