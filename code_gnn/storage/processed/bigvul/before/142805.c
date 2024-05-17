MIMETypeRegistry::SupportsType HTMLMediaElement::GetSupportsType(
    const ContentType& content_type) {
  DEFINE_STATIC_LOCAL(const String, codecs, ("codecs"));

  static base::debug::CrashKeyString* content_type_crash_key =
      base::debug::AllocateCrashKeyString("media_content_type",
                                          base::debug::CrashKeySize::Size256);
  base::debug::ScopedCrashKeyString scoped_crash_key(
      content_type_crash_key, content_type.Raw().Utf8().data());

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
