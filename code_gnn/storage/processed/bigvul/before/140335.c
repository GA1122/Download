static HTMLImageElement* imageElementFromImageDocument(Document* document) {
  if (!document)
    return 0;
  if (!document->isImageDocument())
    return 0;

  HTMLElement* body = document->body();
  if (!body)
    return 0;

  Node* node = body->firstChild();
  if (!isHTMLImageElement(node))
    return 0;
  return toHTMLImageElement(node);
}
