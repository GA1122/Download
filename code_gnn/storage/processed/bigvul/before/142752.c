bool CanLoadURL(const KURL& url, const String& content_type_str) {
  DEFINE_STATIC_LOCAL(const String, codecs, ("codecs"));

  ContentType content_type(content_type_str);
  String content_mime_type = content_type.GetType().DeprecatedLower();
  String content_type_codecs = content_type.Parameter(codecs);

  if (content_mime_type.IsEmpty() ||
      content_mime_type == "application/octet-stream" ||
      content_mime_type == "text/plain") {
    if (url.ProtocolIsData())
      content_mime_type = MimeTypeFromDataURL(url.GetString());
  }

  if (content_mime_type.IsEmpty())
    return true;

  if (content_mime_type != "application/octet-stream" ||
      content_type_codecs.IsEmpty()) {
    return MIMETypeRegistry::SupportsMediaMIMEType(content_mime_type,
                                                   content_type_codecs) !=
           MIMETypeRegistry::kIsNotSupported;
  }

  return false;
}
