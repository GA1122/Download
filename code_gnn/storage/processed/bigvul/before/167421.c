HTMLStyleElement* HTMLStyleElement::Create(Document& document,
                                           const CreateElementFlags flags) {
  return new HTMLStyleElement(document, flags);
}
