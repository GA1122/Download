static bool IsArchiveMIMEType(const String& mime_type) {
  return DeprecatedEqualIgnoringCase("multipart/related", mime_type) ||
         DeprecatedEqualIgnoringCase("message/rfc822", mime_type);
}
