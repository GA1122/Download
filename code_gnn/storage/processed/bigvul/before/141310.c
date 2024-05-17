AtomicString Document::contentType() const {
  if (!mime_type_.IsEmpty())
    return mime_type_;

  if (DocumentLoader* document_loader = Loader())
    return document_loader->MimeType();

  String mime_type = SuggestedMIMEType();
  if (!mime_type.IsEmpty())
    return AtomicString(mime_type);

  return AtomicString("application/xml");
}
