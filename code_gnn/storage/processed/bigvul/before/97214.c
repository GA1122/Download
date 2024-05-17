String WebFrameLoaderClient::generatedMIMETypeForURLScheme(const String& URLScheme) const {
  String mimetype("x-apple-web-kit/");
  mimetype.append(URLScheme.lower());
  return mimetype;
}
