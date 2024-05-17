static bool IsArchiveMIMEType(const String& mime_type) {
  return DeprecatedEqualIgnoringCase("multipart/related", mime_type);
}
