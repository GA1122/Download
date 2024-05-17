HTMLFormElement* HTMLFormElement::create(Document& document) {
  UseCounter::count(document, UseCounter::FormElement);
  return new HTMLFormElement(document);
}
