MIMETypeRegistry::SupportsType HTMLMediaElement::supportsType(
    const ContentType& contentType) {
  DEFINE_STATIC_LOCAL(const String, codecs, ("codecs"));

  String type = contentType.type().lower();
  String typeCodecs = contentType.parameter(codecs);

  if (type.isEmpty())
    return MIMETypeRegistry::IsNotSupported;

  if (type == "application/octet-stream")
    return MIMETypeRegistry::IsNotSupported;

  return MIMETypeRegistry::supportsMediaMIMEType(type, typeCodecs);
}
