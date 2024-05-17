HTMLFormElement* HTMLFormElement::Create(Document& document) {
  UseCounter::Count(document, WebFeature::kFormElement);
  return new HTMLFormElement(document);
}
