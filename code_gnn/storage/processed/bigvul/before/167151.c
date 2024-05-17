MIMETypeRegistry::SupportsType HTMLMediaElement::GetSupportsType(
    const ContentType& content_type) {
  DEFINE_STATIC_LOCAL(const String, codecs, ("codecs"));

  String type = content_type.GetType().DeprecatedLower();
  String type_codecs = content_type.Parameter(codecs);

  if (type.IsEmpty())
    return MIMETypeRegistry::kIsNotSupported;

  if (type == "application/octet-stream")
    return MIMETypeRegistry::kIsNotSupported;

  MIMETypeRegistry::SupportsType result =
      MIMETypeRegistry::SupportsMediaMIMEType(type, type_codecs);
  ReportContentTypeResultToUMA(content_type.Raw(), result);
  return result;
}
