SVGStyleElement* SVGStyleElement::Create(Document& document,
                                         const CreateElementFlags flags) {
  return new SVGStyleElement(document, flags);
}
