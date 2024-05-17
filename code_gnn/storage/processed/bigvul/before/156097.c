HTMLLinkElement* HTMLLinkElement::Create(Document& document,
                                         const CreateElementFlags flags) {
  return new HTMLLinkElement(document, flags);
}
