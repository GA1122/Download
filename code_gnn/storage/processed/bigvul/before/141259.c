String Document::SuggestedMIMEType() const {
  if (IsXMLDocument()) {
    if (IsXHTMLDocument())
      return "application/xhtml+xml";
    if (IsSVGDocument())
      return "image/svg+xml";
    return "application/xml";
  }
  if (xmlStandalone())
    return "text/xml";
  if (IsHTMLDocument())
    return "text/html";

  if (DocumentLoader* document_loader = Loader())
    return document_loader->MimeType();
  return String();
}
